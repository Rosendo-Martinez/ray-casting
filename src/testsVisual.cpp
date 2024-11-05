#include <SFML/Graphics.hpp>
#include "RayCasting.h"
#include <iostream>

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
    window.setKeyRepeatEnabled(false);

    std::vector<Point> points;
    std::vector<LineSegment> lineSegments;
    std::vector<Line> lines;

    points.push_back(Point(100,100));
    lines.push_back(Line(PI/2, Point(201,0)));
    lines.push_back(Line(PI - 0.5, Point(100, 500)));
    lines.push_back(Line(0.5, Point(50,50)));
    lineSegments.push_back(LineSegment(Point(50,120), Point(400,700)));

    float scale = 1;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up)
                {
                    scale += .10f;

                    if (scale > 4.f)
                    {
                        scale = 4.f;
                    }
                    std::cout << "Scale: " << scale << "\n";
                }
                if (event.key.code == sf::Keyboard::Down)
                {
                    scale -=.10f;

                    if (scale < .1f)
                    {
                        scale = .1f;
                    }

                    std::cout << "Scale: " << scale << "\n";
                }
            }
        }

        window.clear();

        for (auto p : points)
        {
            drawPoint(Point(p.x * scale, p.y * scale), window);
        }

        for (auto ls : lineSegments)
        {
            drawLineSegment(LineSegment(Point(ls.a.x * scale, ls.a.y * scale), Point(ls.b.x * scale, ls.b.y * scale)), window);
        }

        for (auto l : lines)
        {
            LineSegment ls;

            if (l.type() == VERTICAL)
            {
                ls.a = Point(l.p.x * scale, 0);
                ls.b = Point(l.p.x * scale, window.getSize().y);
            }
            else
            {
                ls.a = Point(0, l.f(0) * scale);
                ls.b = Point(window.getSize().x * scale, l.f(window.getSize().x) * scale);
            }

            drawLineSegment(ls, window);
        }

        window.display();
    }

    return 0;
}