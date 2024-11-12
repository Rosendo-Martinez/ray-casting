#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "RayCasting.h"

class Controller
{
private:
    std::vector<LineSegment> map;
    std::vector<Point> fan;
    Point base = Point(7,5);
    int rayCount = 25;

public:
    Controller() 
    {
        // Load default map

        //outside walls
        LineSegment ab = LineSegment(Point(0,0), Point(0,10));
        LineSegment bc = LineSegment(Point(0,10), Point(15,10));
        LineSegment cd = LineSegment(Point(15,10), Point(15,0));
        LineSegment da = LineSegment(Point(15,0), Point(0,0));

        // inner rectangle
        LineSegment eg = LineSegment(Point(2,1), Point(2,9));
        LineSegment gh = LineSegment(Point(2,9), Point(4,9));
        LineSegment hf = LineSegment(Point(4,9), Point(4,1));
        LineSegment fe = LineSegment(Point(4,1), Point(2,1));

        // inner triangle
        LineSegment jk = LineSegment(Point(9,5), Point(14,8));
        LineSegment kl = LineSegment(Point(14,8), Point(14,2));
        LineSegment lj = LineSegment(Point(14,2), Point(9,5));

        map.push_back(ab);
        map.push_back(bc);
        map.push_back(cd);
        map.push_back(da);
        map.push_back(eg);
        map.push_back(gh);
        map.push_back(hf);
        map.push_back(fe);
        map.push_back(jk);
        map.push_back(kl);
        map.push_back(lj);

        getClosestIntersectionOfRays(base, map, fan);
    };

    void moveBase(Point newBase)
    {
        base = newBase;

        fan.clear();
        getClosestIntersectionOfRays(base, map, fan);
    }

    void changeRayCount(int newRayCount)
    {
        newRayCount = rayCount;

        fan.clear();
        getClosestIntersectionOfRays(base, map, fan);
    }

    const std::vector<LineSegment> & getMap()
    {
        return map;
    }

    const std::vector<Point> & getFan()
    {
        return fan;
    }

    Point getBase()
    {
        return base;
    }
};

class Render 
{
private:
    sf::RenderWindow window;
    sf::Vector2f     windowPOS;
    bool             isDragging;
    sf::Vector2f     windowPOSBeforeDrag;
    sf::Vector2f     mouseClick;
    sf::Vector2f     mouseHold;
    float            m_scale = 50;
    bool isHoldingRightClick = false;
    Controller ctrl;

    Point descale(const Point p) const
    {
        return Point(p.x / m_scale, p.y / m_scale);
    }

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

    Ray scale(const Ray r) const
    {
        return Ray(r.angle, scale(r.base));
    }

    LineSegment toDrawableLineSegment(const Line l) const
    {
        LineSegment ls;

        if (l.type() == LineTypes::Vertical)
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

    LineSegment toDrawableLineSegment(const Ray r) const
    {
        // Ray base is not on screen and can't cross it either
        // Somewhat gimmicky solution
        if ((r.getDirection() == RayDirection::NW || r.getDirection() == RayDirection::SW || r.getDirection() == RayDirection::W) && r.base.x < windowPOS.x)
        {
            return LineSegment(r.base, r.base);
        }
        else if ((r.getDirection() == RayDirection::NE || r.getDirection() == RayDirection::SE || r.getDirection() == RayDirection::E) && r.base.x > windowPOS.x + window.getSize().x)
        {
            return LineSegment(r.base, r.base);
        }
        else if (r.getDirection() == RayDirection::N && r.base.y < windowPOS.y)
        {
            return LineSegment(r.base, r.base);
        }
        else if (r.getDirection() == RayDirection::S && r.base.y > windowPOS.y + window.getSize().y)
        {
            return LineSegment(r.base, r.base);
        }

        LineSegment ls;
        Line l = r.toLine();

        ls.a = r.base;

        if (r.getDirection() == RayDirection::N)
        {
            ls.b = Point(r.base.x, windowPOS.y);
        }
        else if (r.getDirection() == RayDirection::S)
        {
            ls.b = Point(r.base.x, windowPOS.y + window.getSize().y);
        }
        else if (r.getDirection() == RayDirection::NW || r.getDirection() == RayDirection::SW || r.getDirection() == RayDirection::W)
        {
            ls.b = Point(windowPOS.x, l.f(windowPOS.x));
        }
        else if (r.getDirection() == RayDirection::NE || r.getDirection() == RayDirection::SE || r.getDirection() == RayDirection::E)
        {
            ls.b = Point(windowPOS.x + window.getSize().x, l.f(windowPOS.x + window.getSize().x));
        }

        return ls;
    }

    void drawPoint(Point p, sf::Color color = sf::Color::White)
    {
        const int radius = 3;
        sf::CircleShape circle;

        circle.setRadius(radius);
        circle.setPosition(sf::Vector2f(p.x - radius, p.y - radius) - windowPOS);
        circle.setPointCount(50);
        circle.setFillColor(color);

        window.draw(circle);
    }

    void drawLineSegment(LineSegment ls, sf::Color color = sf::Color::White)
    {
        sf::VertexArray lineSegment (sf::Lines, 2);

        lineSegment[0].position = sf::Vector2f(ls.a.x, ls.a.y) - windowPOS;
        lineSegment[1].position = sf::Vector2f(ls.b.x, ls.b.y) - windowPOS;

        lineSegment[0].color = color;
        lineSegment[1].color = color;

        window.draw(lineSegment);
    }

    void userInput()
    {
        if (isDragging)
        {
            mouseHold = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
            windowPOS = windowPOSBeforeDrag;
            windowPOS += mouseClick - mouseHold;
        }

        if (isHoldingRightClick)
        {
            ctrl.moveBase(descale(Point(sf::Mouse::getPosition(window).x + windowPOS.x, sf::Mouse::getPosition(window).y + windowPOS.y)));
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
                    if (m_scale > 10)
                    {
                        m_scale += 10.f;
                    }
                    if (m_scale > 1)
                    {
                        m_scale += 1.f;
                    }
                    else 
                    {
                        m_scale += .10f;
                    }
                }
                if (event.key.code == sf::Keyboard::Down)
                {

                    if (m_scale > 10)
                    {
                        m_scale -= 10.f;
                    }
                    if (m_scale > 1)
                    {
                        m_scale -= 1.f;
                    }
                    else
                    {
                        m_scale -= .10f;
                    }

                    if (m_scale < 0.1f)
                    {
                        m_scale = 0.1f;
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
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    isHoldingRightClick = true;
                }
            }

            if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    isDragging = false;
                }
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    isHoldingRightClick = false;
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

        // Draw axis
        drawLineSegment(toDrawableLineSegment(scale(Line(0, Point(0,0)))), sf::Color(128,128,128, 255));
        drawLineSegment(toDrawableLineSegment(scale(Line(PI/2, Point(0,0)))), sf::Color(128,128,128, 255));

        // Draw map
        for (auto ls : ctrl.getMap())
        {
            drawLineSegment(scale(ls));
        }

        // Draw light source
        drawPoint(scale(ctrl.getBase()), sf::Color(255,255,0,255));

        // Draw fan and closest intersection points
        sf::VertexArray triangleFan(sf::TriangleFan, ctrl.getFan().size() + 2); // +2 for light source point, and for repeating first point on fan
        triangleFan[0] = sf::Vector2f(scale(ctrl.getBase()).x, scale(ctrl.getBase()).y) - windowPOS;
        triangleFan[0].color = sf::Color(255,255,0,100);
        for (int i = 0; i < ctrl.getFan().size(); i++)
        {
            triangleFan[i + 1].position = sf::Vector2f(scale(ctrl.getFan()[i]).x,scale(ctrl.getFan()[i]).y) - windowPOS;
            triangleFan[i + 1].color = sf::Color(255,255,0,100);
        }
        // double up on first point in fan vector to complete actual fan (i.e make it go fully around)
        triangleFan[ctrl.getFan().size() + 1].position = sf::Vector2f(scale(ctrl.getFan()[0]).x,scale(ctrl.getFan()[0]).y) - windowPOS;
        triangleFan[ctrl.getFan().size() + 1].color = sf::Color(255,255,0,100);
        window.draw(triangleFan);

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