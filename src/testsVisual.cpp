#include <SFML/Graphics.hpp>
#include "RayCasting.h"
#include <iostream>


class Render 
{
private:
    sf::RenderWindow window;
    sf::Vector2f     windowPOS;
    bool             isDragging;
    sf::Vector2f     windowPOSBeforeDrag;
    sf::Vector2f     mouseClick;
    sf::Vector2f     mouseHold;
    float            m_scale = 1;

    Point scale(const Point p) const
    {
        return Point(p.x * m_scale, p.y * m_scale);
    }

    Line scale(const Line l) const
    {
        return Line(l.angle, scale(l.p));
    }

    LineSegment scale(const LineSegment ls) const
    {
        return LineSegment(scale(ls.a), scale(ls.b));
    }

    void drawPoint(Point p)
    {
        const int radius = 3;
        sf::CircleShape circle;

        circle.setRadius(radius);
        circle.setPosition(sf::Vector2f(p.x - radius, p.y - radius) - windowPOS);
        circle.setPointCount(50);
        circle.setFillColor(sf::Color::White);

        window.draw(circle);
    }

    void drawLineSegment(LineSegment ls)
    {
        sf::VertexArray lineSegment (sf::Lines, 2);

        lineSegment[0].position = sf::Vector2f(ls.a.x, ls.a.y) - windowPOS;
        lineSegment[1].position = sf::Vector2f(ls.b.x, ls.b.y) - windowPOS;

        window.draw(lineSegment);
    }

    LineSegment toDrawableLineSegment(Line l)
    {
        LineSegment ls;

        if (l.type() == VERTICAL)
        {
            ls.a = Point(l.p.x, windowPOS.y);
            ls.b = Point(l.p.x, windowPOS.y + window.getSize().y);
        }
        else
        {
            ls.a = Point(windowPOS.x, l.f(windowPOS.x));
            ls.b = Point(windowPOS.x + window.getSize().x, l.f(windowPOS.x + window.getSize().x));
        }

        return ls;
    }

    void userInput()
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
                    m_scale += .10f;

                    if (m_scale > 4.f)
                    {
                        m_scale = 4.f;
                    }
                }
                if (event.key.code == sf::Keyboard::Down)
                {
                    m_scale -=.10f;

                    if (m_scale < .1f)
                    {
                        m_scale = .1f;
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
    }

    void render()
    {
        window.clear();

        std::vector<Point> points;
        std::vector<LineSegment> lineSegments;
        std::vector<Line> lines;

        points.push_back(Point(0,0));
        lines.push_back(Line(0, Point(0,0)));
        lines.push_back(Line(PI/2, Point(0,0)));

        for (auto p : points)
        {
            drawPoint(scale(p));
        }

        for (auto ls : lineSegments)
        {
            drawLineSegment(scale(ls));
        }

        for (auto l : lines)
        {
            drawLineSegment(toDrawableLineSegment(scale(l)));
        }

        window.display();
    }

public:

    Render() 
    {
        window.create(sf::VideoMode(1200, 800), "Window", sf::Style::Resize);
        window.setKeyRepeatEnabled(false);

        windowPOS = sf::Vector2f(0.f,0.f);
        isDragging = false;
        windowPOSBeforeDrag = windowPOS;


    };

    void run() 
    {
        while (window.isOpen())
        {
            userInput();
            render();
        }
    };
};


int main()
{
    Render r;
    r.run();

    return 0;
}