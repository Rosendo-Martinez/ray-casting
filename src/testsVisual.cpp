#include <SFML/Graphics.hpp>
#include "RayCasting.h"

// g++ -c visual.cpp
// g++ visual.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system

void drawPoint(Point p, sf::RenderWindow & window)
{
    const int radius = 3;
    sf::CircleShape circle;

    circle.setRadius(radius);
    circle.setPosition(sf::Vector2f(p.x + radius, p.y + radius));
    circle.setPointCount(50);
    circle.setFillColor(sf::Color::White);

    window.draw(circle);
}

void drawLine(Line l, sf::RenderWindow & window)
{
    sf::VertexArray line (sf::Lines, 2);

    if (l.type() == VERTICAL)
    {
        line[0].position = sf::Vector2f(l.xIntercept(),0);
        line[1].position = sf::Vector2f(l.xIntercept(),window.getSize().y);
    }
    else
    {
        line[0].position = sf::Vector2f(0, l.f(0));
        line[1].position = sf::Vector2f(window.getSize().x, l.f(window.getSize().x));
    }

    window.draw(line);
}

void drawLineSegment(LineSegment ls, sf::RenderWindow & window)
{
    sf::VertexArray lineSegment (sf::Lines, 2);

    lineSegment[0].position = sf::Vector2f(ls.a.x, ls.a.y);
    lineSegment[1].position = sf::Vector2f(ls.b.x, ls.b.y);

    window.draw(lineSegment);
}

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

        window.clear();
        drawPoint(Point(100,100), window);
        drawLine(Line(PI/2, Point(201,0)), window);
        drawLine(Line(PI - 0.5, Point(100, 500)), window);
        drawLineSegment(LineSegment(Point(50,120), Point(400,700)), window);
        window.display();
    }

    return 0;
}