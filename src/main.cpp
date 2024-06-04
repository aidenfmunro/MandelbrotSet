
#include <immintrin.h>

#include "mandelbrot.hpp"
#include "perftest.hpp"
#include "constants.hpp"
#include "settings.hpp"

int main(void)
{
    sf::Uint8* pixels = new sf::Uint8[WINDOW_WIDTH * WINDOW_HEIGHT * BYTES_IN_PIXEL];

    int shiftX = WINDOW_WIDTH  / 2;
    int shiftY = WINDOW_HEIGHT / 2;
    float zoom = 1/300.f;

    // GRAPHICS_ON(

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
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Z)
                {
                    zoom /= 2;
                }
                else if (event.key.code == sf::Keyboard::D)
                {
                    shiftX += 10;
                }
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

    // )

    for (int i = 0; i < 10; i++)
    {
        uint64_t dtavx = perfMandelbrotTest (generateMandelbrotSetAVX, pixels, 10) / 100000000;
        uint64_t dtnop = perfMandelbrotTest (generateMandelbrotSet,    pixels, 10) / 100000000;
    
        printf("AVX time: %ld\n",     dtavx);

        printf("Default time: %ld\n", dtnop);

        double compare = (double) dtnop / (double) dtavx;

        printf("Boost: %lg\n", compare);
    }

    delete[] pixels;

    return 0;
}