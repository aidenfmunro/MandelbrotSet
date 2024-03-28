#ifndef MANDELBROT_HPP
#define MANDELBROT_HPP

#include <SFML/Graphics.hpp>

void generateMandelbrotSet    (sf::Uint8* pixels, int shiftX, int shiftY, float zoom);

void generateMandelbrotSetAVX (sf::Uint8* pixels, int shiftX, int shiftY, float zoom);

void setPixel                 (sf::Uint8* pixels, int screenX, int screenY, int iteration);

#endif