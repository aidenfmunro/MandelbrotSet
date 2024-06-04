#include <immintrin.h>
#include <SFML/Graphics.hpp>
#include <math.h>

#include "mandelbrot.hpp"
#include "constants.hpp"

struct complexNumber
{
    float real;
    float imag;
};

[[deprecated]]

void generateMandelbrotSet(sf::Uint8* pixels, int shiftX, int shiftY, float zoom)
{
    for (int screenY = 0; screenY < WINDOW_HEIGHT; screenY++)
    {
        float y0 = ( (float)screenY - shiftY ) * zoom;
        float x0 = (                - shiftX ) * zoom;

        for (int screenX = 0; screenX < WINDOW_WIDTH; screenX++)
        {
            x0 += zoom;

            float x = x0;
            float y = y0;

            int iteration = 0; 

            for (; iteration < MAX_ITERATION_DEPTH; iteration++)
            {

                float x2 = x * x;
                float y2 = y * y;
                float xy = x * y;

                x = x2 - y2 + x0;
                y = xy + xy + y0;


                if (x2 + y2 > MAX_RADIUS * MAX_RADIUS)
                    break;
            }

            setPixel(pixels, screenX, screenY, iteration);
        }
    }
}




void generateMandelbrotSetAVX(sf::Uint8* pixels, int shiftX, int shiftY, float zoom)
{

    __m256 _STEPS = _mm256_set_ps  (7, 6, 5, 4, 3, 2, 1, 0);
    __m256 _MASK  = _mm256_set1_ps (1);
    __m256 _R2    = _mm256_set1_ps (MAX_RADIUS * MAX_RADIUS);
    __m256 _X0    = _mm256_mul_ps(_mm256_add_ps(_mm256_set1_ps(- shiftX), _STEPS), _mm256_set1_ps(zoom));

    float dx = 8 * zoom;

    for (int screenY = 0; screenY < WINDOW_HEIGHT; screenY++)
    {
        float   y0 = ( ( (float)screenY - shiftY ) * zoom );

        __m256 _y0 = _mm256_set1_ps(y0);

        __m256 _x0 = _X0;

        for (int screenX = 0; screenX < WINDOW_WIDTH; screenX += 8)
        {         
            _x0 = _mm256_add_ps(_x0, _mm256_set1_ps(dx));

            int      iteration  = 0;
            __m256i _iterations = _mm256_setzero_si256();

            __m256 _x = _x0;
            __m256 _y = _y0;
            
            for (; iteration < MAX_ITERATION_DEPTH; iteration++)
            {
                __m256 _x2 = _mm256_mul_ps(_x, _x);
                __m256 _y2 = _mm256_mul_ps(_y, _y);
                __m256 _xy = _mm256_mul_ps(_x, _y);

                __m256 _r2 = _mm256_add_ps(_x2, _y2);

                __m256 _cmp = _mm256_cmp_ps(_r2, _R2, _CMP_LT_OS);

                if (! _mm256_movemask_ps(_cmp))
                    break;

                _x = _mm256_add_ps(_mm256_sub_ps(_x2, _y2), _x0);
                _y = _mm256_add_ps(_mm256_add_ps(_xy, _xy), _y0);
                
                _iterations = _mm256_add_epi32(_mm256_cvtps_epi32(_mm256_and_ps(_cmp, _MASK)), _iterations);
            }   

            int* iterations = (int*)&_iterations; // TODO: use author convertation to int *

            for (int i = 0; i < 8; i++)
            {
                setPixel(pixels, screenX + i, screenY, iterations[i]);
            }
        }
    }
}

void setPixel(sf::Uint8* pixels, int screenX, int screenY, int iteration)
{
    sf::Uint8 r = 0, g = 0, b = 0;

    if (iteration < MAX_ITERATION_DEPTH)
    {
        float iterColor = iteration;

        r = (sf::Uint8)(iterColor / 2);
        g = (sf::Uint8)(iterColor * 2);
        b = (sf::Uint8)(iterColor * 2 + 4);
    }

    int pixelIndex = (screenY * WINDOW_WIDTH + screenX) * BYTES_IN_PIXEL;

    pixels[pixelIndex + 0] = r;
    pixels[pixelIndex + 1] = g;
    pixels[pixelIndex + 2] = b;
    pixels[pixelIndex + 3] = 255;

}