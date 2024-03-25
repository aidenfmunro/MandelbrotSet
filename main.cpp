#include <SFML/Graphics.hpp>

void drawMandelbrot(sf::Uint8* pixels);

const int WINDOW_HEIGHT = 1080;
const int WINDOW_WIDTH  = 1920;

const int   MAX_ITERARTION_DEPTH = 256;
const float MAX_RADIUS           = 10;

int main()
{
    sf::Uint8* pixels = new sf::Uint8[WINDOW_WIDTH * WINDOW_HEIGHT * 4];

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
                            "Mandelbrot Set", 
                            sf::Style::Default);

    sf::Texture texture;
    texture.create(WINDOW_WIDTH, WINDOW_HEIGHT);
    texture.update(pixels);

    sf::Sprite sprite(texture);



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

        drawMandelbrot(pixels);
        texture.update(pixels);

        window.clear();
        window.draw(sprite);
        window.display();

    }

    delete[] pixels;

    return 0;
}

void drawMandelbrot(sf::Uint8* pixels)
{
    const float r2 = MAX_RADIUS * MAX_RADIUS;

    const float dx = 1 / WINDOW_WIDTH;
    const float dy = 1 / WINDOW_HEIGHT;

    float cx = 0.f, cy = 0.f, scale = 1.f;

    for (int iy = 0; iy < WINDOW_HEIGHT; iy++, cy += dy)
    {
        float x0 = 
        for (int ix = 0; ix < WINDOW_WIDTH; ix++, cx += dx)
        {
            float x = 0.f;
            float y = 0.f;

            int iter = 0;

            float x2 = 0.f;
            float y2 = 0.f;

            while (x2 + y2 < r2 && iter < MAX_ITERARTION_DEPTH)
            {
                x = x2 - y2   + cx;
                y = 2 * x * y + cy;

                x2 = x * x;
                y2 = y * y;

                iter++;
            }

            sf::Uint8 r = 0, g = 0, b = 0;

            if (iter < MAX_ITERARTION_DEPTH)
            {
                float iterColor = iter * 255 / MAX_ITERARTION_DEPTH;

                r = (sf::Uint8)(255 - iterColor);
                g = (sf::Uint8)(iterColor + 2);
                b = (sf::Uint8)(iterColor + 3);
            }

            int pixelIndex = (iy * WINDOW_WIDTH + ix) * 4;

            pixels[pixelIndex + 0] = r;
            pixels[pixelIndex + 1] = g;
            pixels[pixelIndex + 2] = b;
            pixels[pixelIndex + 3] = 255;
        }
    }
    
}
