#include "Map.h"
#include <iostream>
#include <string>

void printTest(const std::string& testDescription, bool result)
{
    if (result)
    {
        std::cout << "  PASSED: ";
    }
    else
    {
        std::cout << "  FAILED: ";
    }

    std::cout << testDescription << "\n";
}

int main(int argc, char* argv[]) 
{
    {
        Map m;
        
        std::cout << "TEST: addLineSegment()\n";
        printTest("check that can add line segment to empty map", m.addLineSegment(LineSegment(Point(0,0), Point(10,10))) == true);
        printTest("check that can add line segment to not empty map", m.addLineSegment(LineSegment(Point(20,20), Point(30,30))) == true);
        printTest("check that can't add duplicate", m.addLineSegment(LineSegment(Point(0,0), Point(10,10))) == false);
    }
    {
        Map m;

        m.addLineSegment(LineSegment(Point(0,0), Point(10,10)));
        m.addLineSegment(LineSegment(Point(20,20), Point(30,30)));

        std::cout << "TEST: removeLineSegment()\n";
        printTest("check that can remove line segment", (m.removeLineSegment(LineSegment(Point(0,0), Point(10,10))) == true && m.sizeLineSegments() == 1));
        printTest("check that can remove non-exist line segment", (m.removeLineSegment(LineSegment(Point(0,0), Point(10,10))) == false && m.sizeLineSegments() == 1));
    }
    {
        std::cout << "TEST: moveEndPoint()\n";
        {
            Map m;

            Point a = Point(0,0);
            Point oldB = Point(10,10);
            Point newB = Point(20,20);

            m.addLineSegment(LineSegment(a, oldB));

            printTest("move 1 endpoint of line segment (ls) [only 1 ls in map, and check other endpoint of ls doesn't change]", (m.moveEndPoint(oldB, newB) && m.getLineSegments()[0].a == a && m.getLineSegments()[0].b == newB));
        }
        {
            Map m;

            Point p = Point(10,10);

            LineSegment ls1 = LineSegment(Point(0,0), p);
            LineSegment ls2 = LineSegment(p, Point(20,20));


            m.addLineSegment(ls1);
            m.addLineSegment(ls2);

            Point newEndpoint = Point(20,10);

            bool result = (m.moveEndPoint(p, newEndpoint) && m.getLineSegments()[0].b == newEndpoint && m.getLineSegments()[1].a == newEndpoint);

            printTest("move endpoint that is endpoint of two line segments [check that it moves for both]", result);
        }
    }
    std::cout << "TEST: closestEndpoint()\n";
    {
        Map m;
        Point p = Point(10,0);
        Point center = Point(0,0);
        float maxDist = 20;

        m.addLineSegment(LineSegment(p, Point(100,100)));

        Point pResult;
        m.closestEndPoint(center, maxDist, pResult);

        printTest("point within max distance", m.closestEndPoint(center, maxDist, pResult) && pResult == p);
    }
    {
        Map m;
        m.addLineSegment(LineSegment(Point(100,100), Point(200,200)));

        float maxDist = 20;
        Point center = Point(0,0);
        Point pResult;

        printTest("no endpoint within max distance", m.closestEndPoint(center, maxDist, pResult) == false);
    }

    return 0;
}