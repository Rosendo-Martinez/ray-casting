#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "RayCasting.h"


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
    std::vector<LineSegment> map;
    std::vector<Ray> rays;
    std::vector<Point> intersectionPoints;
    std::vector<LineSegment> intersectionLineSegments;
    std::vector<Point> fan;
    Point rayBase = Point(7,5);
    bool isHoldingRightClick = false;

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

    void loadMap()
    {
        // eventual I'd like to load from a file, for now just hard code it.
        
        // outside walls
        LineSegment ab = LineSegment(Point(0,0), Point(0,10));
        LineSegment bc = LineSegment(Point(0,10), Point(15,10));
        LineSegment cd = LineSegment(Point(15,10), Point(15,0));
        LineSegment da = LineSegment(Point(15,0), Point(0,0));

        // inner rect
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


        rays = getIntersectionsOfRays(rayBase, 10, map, intersectionPoints, intersectionLineSegments);
        getClosestIntersectionsOfRays(rayBase, 100, map, fan);
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
            rays.clear();
            intersectionPoints.clear();
            intersectionLineSegments.clear();
            fan.clear();

            // Descale mouse pos and window pos
            rayBase = Point(sf::Mouse::getPosition(window).x/m_scale + windowPOS.x/m_scale, sf::Mouse::getPosition(window).y/m_scale + windowPOS.y/m_scale);

            rays = getIntersectionsOfRays(rayBase, 100, map, intersectionPoints, intersectionLineSegments);
            getClosestIntersectionsOfRays(rayBase, 100, map, fan);
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
        // drawPoint(scale(Point(0,0)));
        drawLineSegment(toDrawableLineSegment(scale(Line(0, Point(0,0)))), sf::Color(128,128,128, 255));
        drawLineSegment(toDrawableLineSegment(scale(Line(PI/2, Point(0,0)))), sf::Color(128,128,128, 255));
        // drawLineSegment(toDrawableLineSegment(scale(Ray(PI/4, Point(100,100)))));
        // drawPoint(scale(Point(100,100)));

        // std::vector<Point> points;
        // std::vector<LineSegment> lineSegments;
        // std::vector<Line> lines;

        for (auto ls : map)
        {
            drawLineSegment(scale(ls));
        }

        // for (auto r : rays)
        // {
        //     drawLineSegment(toDrawableLineSegment(scale(r)), sf::Color(255, 255, 0, 100));
        // }

        drawPoint(scale(rayBase), sf::Color(255,255,0,255));

        // Size is fan size plus 1 for ray base
        sf::VertexArray triangleFan(sf::TriangleFan, fan.size() + 2);
        triangleFan[0] = sf::Vector2f(scale(rayBase).x, scale(rayBase).y) - windowPOS;
        triangleFan[0].color = sf::Color(255,255,0,100);

        for (int i = 0; i < fan.size(); i++)
        {
            triangleFan[i + 1].position = sf::Vector2f(scale(fan[i]).x,scale(fan[i]).y) - windowPOS;

            triangleFan[i + 1].color = sf::Color(255,255,0,50);
        }

        // double up on first point in fan vector to complete actual fan (i.e make it go fully around)
        triangleFan[fan.size() + 1].position = sf::Vector2f(scale(fan[0]).x,scale(fan[0]).y) - windowPOS;
        triangleFan[fan.size() + 1].color = sf::Color(255,255,0,50);

        window.draw(triangleFan);

        for (auto p : fan)
        {
            drawPoint(scale(p), sf::Color(255, 0, 0, 200));
            drawLineSegment(scale(LineSegment(rayBase, p)), sf::Color(255, 255, 0, 100));
        }



        // for (auto p : intersectionPoints)
        // {
        //     drawPoint(scale(p), sf::Color(255, 0, 0, 200));
        // }

        // for (auto ls : intersectionLineSegments)
        // {
        //     drawLineSegment((scale(ls)));
        // }

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

        loadMap();
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