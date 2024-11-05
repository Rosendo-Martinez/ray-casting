#include <SFML/Graphics.hpp>
#include "RayCasting.h"
#include <iostream>

// g++ -c visual.cpp
// g++ visual.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system

void drawPoint(Point p, sf::RenderWindow & window, sf::Vector2f windowPOS)
{
    const int radius = 3;
    sf::CircleShape circle;

    circle.setRadius(radius);
    circle.setPosition(sf::Vector2f(p.x - radius, p.y - radius) - windowPOS);
    circle.setPointCount(50);
    circle.setFillColor(sf::Color::White);

    window.draw(circle);
}

void drawLineSegment(LineSegment ls, sf::RenderWindow & window, sf::Vector2f windowPOS)
{
    sf::VertexArray lineSegment (sf::Lines, 2);

    lineSegment[0].position = sf::Vector2f(ls.a.x, ls.a.y) - windowPOS;
    lineSegment[1].position = sf::Vector2f(ls.b.x, ls.b.y) - windowPOS;

    window.draw(lineSegment);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Window", sf::Style::Resize);
    sf::Vector2f windowPOS(0.f,0.f);
    window.setKeyRepeatEnabled(false);

    bool isDragging = false;
    sf::Vector2f windowPOSBeforeDrag = windowPOS;
    sf::Vector2f mouseClick;
    sf::Vector2f mouseHold;

    std::vector<Point> points;
    std::vector<LineSegment> lineSegments;
    std::vector<Line> lines;

    points.push_back(Point(0,0));
    points.push_back(Point(window.getSize().x, 0));
    points.push_back(Point(window.getSize().x, window.getSize().y));
    points.push_back(Point(0, window.getSize().y));
    lines.push_back(Line(PI/4, Point(0,0)));
    lines.push_back(Line(PI/2, Point(window.getSize().x, 0)));
    lines.push_back(Line(PI, Point(0,window.getSize().y)));
    lines.push_back(LineSegment(Point(window.getSize().x, 0),Point(0, window.getSize().y)).toLine());
    lines.push_back(LineSegment(Point(0, 0),Point(window.getSize().x, window.getSize().y)).toLine());

    float scale = 1;

    while (window.isOpen())
    {
        if (isDragging)
        {
            mouseHold = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
            windowPOS = windowPOSBeforeDrag;
            windowPOS += mouseClick - mouseHold;
        }

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
                }
                if (event.key.code == sf::Keyboard::Down)
                {
                    scale -=.10f;

                    if (scale < .1f)
                    {
                        scale = .1f;
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    isDragging = true;
                    windowPOSBeforeDrag = windowPOS;
                    mouseClick = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
                }
            }

            if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    isDragging = false;
                }
            }

            // Detect window resizing
            if (event.type == sf::Event::Resized) 
            {
                sf::Vector2u newSize = window.getSize();

                // Update the view to match the new window size
                sf::FloatRect visibleArea(0, 0, newSize.x, newSize.y);
                window.setView(sf::View(visibleArea));
            }
        }

        window.clear();

        for (auto p : points)
        {
            drawPoint(Point(p.x * scale, p.y * scale), window, windowPOS);
        }

        for (auto ls : lineSegments)
        {
            drawLineSegment(LineSegment(Point(ls.a.x * scale, ls.a.y * scale), Point(ls.b.x * scale, ls.b.y * scale)), window, windowPOS);
        }

        for (auto l : lines)
        {
            Line scaledLine(l.angle, Point(l.p.x * scale, l.p.y * scale));
            LineSegment ls;

            if (l.type() == VERTICAL)
            {
                ls.a = Point(scaledLine.p.x, windowPOS.y);
                ls.b = Point(scaledLine.p.x, windowPOS.y + window.getSize().y);
            }
            else
            {
                ls.a = Point(windowPOS.x, scaledLine.f(windowPOS.x));
                ls.b = Point(windowPOS.x + window.getSize().x, scaledLine.f(windowPOS.x + window.getSize().x));
            }

            drawLineSegment(ls, window, windowPOS);
        }

        window.display();
    }

    return 0;
}