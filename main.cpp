#include <SFML/Graphics.hpp>
#include <immintrin.h>

void generateMandelbrotSet   (sf::Uint8* pixels, int shiftX, int shiftY, float zoom);
void generateMandelbrotSetAVX(sf::Uint8* pixels, int shiftX, int shiftY, float zoom);

void setPixel(sf::Uint8* pixels, int screenX, int screenY, int iteration);

const int WINDOW_HEIGHT = 1080;
const int WINDOW_WIDTH  = 1920;

const int BYTES_IN_PIXEL = 4;

const int MAX_ITERATION_DEPTH = 256;
const int MAX_RADIUS = 10;

struct complexNumber
{
    float real;
    float imag;
};

int main(void)
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Mandelbrot Set");

    sf::Uint8* pixels = new sf::Uint8[WINDOW_WIDTH * WINDOW_HEIGHT * BYTES_IN_PIXEL];

    sf::Texture screen;
    screen.create(WINDOW_WIDTH, WINDOW_HEIGHT);

    sf::Sprite sprite(screen);

    float zoom = 1/300.f;

    int shiftX = WINDOW_WIDTH  / 2;
    int shiftY = WINDOW_HEIGHT / 2;

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            sf::Vector2i position = sf::Mouse::getPosition(window);
            shiftX -= position.x - shiftX;
            shiftY -= position.y - shiftY;

            zoom *= 2;
        }
    
        // generateMandelbrotSet(pixels, shiftX, shiftY, zoom);
        generateMandelbrotSetAVX(pixels, shiftX, shiftY, zoom);
        screen.update(pixels);

        window.clear();
        window.draw(sprite);
        window.display();

    }

    unsigned long long t1 = __rdtsc();

    for (int i = 0; i < 100; i++)
    {
    generateMandelbrotSetAVX(pixels, shiftX, shiftY, zoom);
    }

    unsigned long long t2 = __rdtsc();

    printf("AVX time: %d", (t2 - t1) / 10000000);

    t1 = __rdtsc();
    
    for (int j = 0; j < 100; j++)
    {
    generateMandelbrotSet   (pixels, shiftX, shiftY, zoom);
    }

    t2 = __rdtsc();

    printf("Default time: %d", (t2 - t1) / 10000000);

    return 0;
}

void perfFuncMandelbrot(sf::Uint8* pixels, void (*function)(sf::Uint8*, int, int, float), int sampleSize)
{

}

void generateMandelbrotSet(sf::Uint8* pixels, int shiftX, int shiftY, float zoom)
{
    for (int screenY = 0; screenY < WINDOW_HEIGHT; screenY++)
    {
        for (int screenX = 0; screenX < WINDOW_WIDTH; screenX++)
        {
            float x = ( (float)screenX - shiftX ) * zoom;
            float y = ( (float)screenY - shiftY ) * zoom;

            complexNumber c = {.real = x, .imag = y};

            complexNumber z = c;

            int iteration = 0; 

            for (; iteration < MAX_ITERATION_DEPTH; iteration++)
            {
                complexNumber z2 = {.real = z.real * z.real - z.imag * z.imag + c.real,
                                    .imag = 2 * z.real * z.imag               + c.imag};

                z = z2;

                iteration++;

                if (z.real * z.real + z.imag * z.imag > MAX_RADIUS * MAX_RADIUS)
                    break;
            }

            setPixel(pixels, screenX, screenY, iteration);
        }
    }
}

struct complexNumberAVX
{
    __m256 real;
    __m256 imag;  
};

const __m256 STEPS = _mm256_set_ps  (7, 6, 5, 4, 3, 2, 1, 0);
const __m256 MASK  = _mm256_set1_ps (1);
const __m256 R2    = _mm256_set1_ps (MAX_RADIUS * MAX_RADIUS);

void generateMandelbrotSetAVX(sf::Uint8* pixels, int shiftX, int shiftY, float zoom)
{
    for (int screenY = 0; screenY < WINDOW_HEIGHT; screenY++)
    {
        for (int screenX = 0; screenX < WINDOW_WIDTH; screenX += 8)
        {
            __m256 _x = _mm256_set1_ps(  (float)screenX - shiftX);
                   _x = _mm256_mul_ps (_mm256_add_ps(_x , STEPS), _mm256_set1_ps(zoom));

            __m256 _y = _mm256_set1_ps(( (float)screenY - shiftY ) * zoom);

            complexNumberAVX c = {.real = _x, .imag = _y};

            int      iteration  = 0;
            __m256i _iterations = _mm256_setzero_si256();
            
            for (; iteration < MAX_ITERATION_DEPTH; iteration++)
            {
                __m256 _x2 = _mm256_mul_ps(_x, _x);
                __m256 _y2 = _mm256_mul_ps(_y, _y);
                __m256 _xy = _mm256_mul_ps(_x, _y);

                __m256 r2 = _mm256_add_ps(_x2, _y2);

                __m256 cmp = _mm256_cmp_ps(r2, R2, _CMP_LT_OS);

                if (! _mm256_movemask_ps(cmp))
                    break;

                _x = _mm256_add_ps(_mm256_sub_ps(_x2, _y2), c.real);
                _y = _mm256_add_ps(_mm256_add_ps(_xy, _xy), c.imag);
                
                _iterations = _mm256_add_epi32(_mm256_cvtps_epi32(_mm256_and_ps(cmp, MASK)), _iterations);
            }   

            int* array = (int*)&_iterations;

            for (int i = 0; i < 8; i++)
            {
                setPixel(pixels, screenX + i, screenY, array[i]);
            }
        }
    }
}

void setPixel(sf::Uint8* pixels, int screenX, int screenY, int iteration)
{
    sf::Uint8 r = 0, g = 0, b = 0;

    if (iteration < MAX_ITERATION_DEPTH)
    {
        float iterColor = iteration * 255.0f / MAX_ITERATION_DEPTH;

        r = (sf::Uint8)(255 - iterColor);
        g = (sf::Uint8)(iterColor + 2);
        b = (sf::Uint8)(iterColor + 3);
    }

    int pixelIndex = (screenY * WINDOW_WIDTH + screenX) * BYTES_IN_PIXEL;

    pixels[pixelIndex + 0] = r;
    pixels[pixelIndex + 1] = g;
    pixels[pixelIndex + 2] = b;
    pixels[pixelIndex + 3] = 255;

}