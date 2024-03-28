#include <SFML/Graphics.hpp>
#include <immintrin.h>

#include "perftest.hpp"
#include "constants.hpp"

uint64_t perfMandelbrotTest(mandelbortFunction_t mandelbortFunction, sf::Uint8* pixels, int sampleSize)
{
    float zoom = 1 / 300;

    int shiftX = WINDOW_WIDTH / 2;
    int shiftY = WINDOW_HEIGHT / 2;

    uint64_t t1 = __rdtsc();
    for (int i = 0; i < sampleSize; i++)
    {
        mandelbortFunction(pixels, shiftX, shiftY, zoom);
    }
    uint64_t t2 = __rdtsc();

    return t2 - t1; 
}