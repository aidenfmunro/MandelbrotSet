#ifndef PERFTEST_HPP
#define PERFTEST_HPP

#include <SFML/Graphics.hpp>

typedef void (*mandelbortFunction_t)(sf::Uint8*, int, int, float);

uint64_t perfMandelbrotTest(mandelbortFunction_t mandelbortFunction, sf::Uint8* pixels, int sampleSize);

#endif