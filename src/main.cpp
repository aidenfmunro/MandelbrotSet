#include <SFML/Graphics.hpp>
#include <immintrin.h>

#include "mandelbrot.hpp"
#include "constants.hpp"
#include "settings.hpp"

int main(void)
{
    sf::Uint8* pixels = new sf::Uint8[WINDOW_WIDTH * WINDOW_HEIGHT * BYTES_IN_PIXEL];

    int shiftX = WINDOW_WIDTH  / 2;
    int shiftY = WINDOW_HEIGHT / 2;
    float zoom = 1/300.f;

    GRAPHICS_ON(

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Mandelbrot Set");

    sf::Texture screen;
    screen.create(WINDOW_WIDTH, WINDOW_HEIGHT);

    sf::Sprite sprite(screen);

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

            zoom /= 2;
        }
    
        generateMandelbrotSetAVX(pixels, shiftX, shiftY, zoom);
        screen.update(pixels);

        window.clear();
        window.draw(sprite);
        window.display();

    }

    )
    unsigned long long t1 = __rdtsc();

    for (int i = 0; i < 10; i++)
    {
    generateMandelbrotSetAVX(pixels, shiftX, shiftY, zoom);
    }

    unsigned long long t2 = __rdtsc();

    double avx = t2 - t1;

    printf("AVX time: %d", (t2 - t1) / 10000000);

    t1 = __rdtsc();
    
    for (int j = 0; j < 10; j++)
    {
    generateMandelbrotSet   (pixels, shiftX, shiftY, zoom);
    }

    t2 = __rdtsc();

    double noop = t2 - t1;

    printf("Default time: %d %lg", (t2 - t1) / 10000000, noop / avx);

    delete[] pixels;

    return 0;
}