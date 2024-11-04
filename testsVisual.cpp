#include <SFML/Graphics.hpp>

// g++ -c visual.cpp
// g++ visual.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system

int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Window");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }

    return 0;
}