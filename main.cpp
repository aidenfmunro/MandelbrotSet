#include <SFML/Graphics.hpp>

void generateMandelbrotSet(sf::Uint8* pixels, int shiftX, int shiftY, float zoom);

const int WINDOW_HEIGHT = 1080;
const int WINDOW_WIDTH  = 1920;

const int BYTES_IN_PIXEL = 4;

const int MAX_ITERATION_DEPTH = 256;

struct complexNumber
{
    long double real;
    long double imag;
};

int main(void)
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Mandelbrot Set");

    sf::Uint8* pixels = new sf::Uint8[WINDOW_WIDTH * WINDOW_HEIGHT * BYTES_IN_PIXEL];

    sf::Texture screen;
    screen.create(WINDOW_WIDTH, WINDOW_HEIGHT);

    sf::Sprite sprite(screen);

    float zoom = 300.f;

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

    generateMandelbrotSet(pixels, shiftX, shiftY, zoom);

    screen.update(pixels);

    window.clear();
    window.draw(sprite);
    window.display();

    }

    return 0;
}

void generateMandelbrotSet(sf::Uint8* pixels, int shiftX, int shiftY, float zoom)
{
    for(int screenY = 0; screenY < WINDOW_HEIGHT; screenY++)
        {
            for (int screenX = 0; screenX < WINDOW_WIDTH; screenX++)
            {
                //scale the pixel location to the complex plane for calculations
                float x = ( (float)screenX - shiftX ) / zoom;
                float y = ( (float)screenY - shiftY ) / zoom;

                complexNumber c = {.real = x, .imag = y};

                complexNumber z = c;

                int iteration = 0; //keep track of the number of iterations

                for (; iteration < MAX_ITERATION_DEPTH; iteration++)
                {
                    complexNumber z2 = {.real = z.real * z.real - z.imag * z.imag + c.real,
                                        .imag = 2 * z.real * z.imag               + c.imag};

                    z = z2;

                    iteration++;

                    if (z.real * z.real + z.imag * z.imag > 100)
                        break;
                }

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
        }
}