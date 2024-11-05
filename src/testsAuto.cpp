#include <iostream>
#include <vector>
#include <string>
#include "RayCasting.h"


void testClosestPointOnRay(Ray r, std::vector<Point> & points, const Point actual, std::string description)
{
    Point result = r.closestPointOnRay(points);

    if (result == actual)
    {
        std::cout << "PASSED: " << description << "\n";
        std::cout << "    Result = (" << result.x << ", " << result.y << ")\n";
        std::cout << "    Actual = (" << actual.x << ", " << actual.y << ")\n";
    }
    else 
    {
        std::cout << "FAILED: " << description << "\n";
        std::cout << "    Result = (" << result.x << ", " << result.y << ")\n";
        std::cout << "    Actual = (" << actual.x << ", " << actual.y << ")\n";
    }
}

void testGetIntersections(const Ray r, const std::vector<LineSegment>& lineSegments, std::vector<Point>& actualIntersectionPoints, std::vector<LineSegment>& actualIntersectionLineSegments, const std::string description)
{
    std::vector<Point> resultIntersectionPoints;
    std::vector<LineSegment> resultIntersectionLineSegments;

    getIntersections(r, lineSegments, resultIntersectionPoints, resultIntersectionLineSegments);


    if (resultIntersectionLineSegments.size() != actualIntersectionLineSegments.size())
    {
        std::cout << "Test: FAILED [intersection line segments wrong]\n" << "    Description: " << description << "\n";

        std::cout << "    Result Intersection LineSegments: "; 
        for (auto resultLs : resultIntersectionLineSegments)
        {
            std::cout << "{(" << resultLs.a.x << ", " << resultLs.a.y << "),(" << resultLs.b.x << ", " << resultLs.b.y << ")} "; 
        }
        std::cout << "\n";

        std::cout << "    Result Intersection Points: ";
        for (auto resultP : resultIntersectionPoints)
        {
            std::cout << "(" << resultP.x << ", " << resultP.y << ") ";
        }
        std::cout << "\n";
        return;
    }

    if (resultIntersectionPoints.size() != actualIntersectionPoints.size())
    {
        std::cout << "Test: FAILED [intersection points wrong]\n" << "    Description: " << description << "\n";

        std::cout << "    Result Intersection LineSegments: "; 
        for (auto resultLs : resultIntersectionLineSegments)
        {
            std::cout << "{(" << resultLs.a.x << ", " << resultLs.a.y << "),(" << resultLs.b.x << ", " << resultLs.b.y << ")} "; 
        }
        std::cout << "\n";

        std::cout << "    Result Intersection Points: ";
        for (auto resultP : resultIntersectionPoints)
        {
            std::cout << "(" << resultP.x << ", " << resultP.y << ") ";
        }
        std::cout << "\n";
        return;
    }

    for (const Point p : actualIntersectionPoints)
    {
        for (auto it = resultIntersectionPoints.begin(); it != resultIntersectionPoints.end(); ++it)
        {
            if (*it == p)
            {
                resultIntersectionPoints.erase(it);
                break;
            }
            else if (it + 1 == resultIntersectionPoints.end())
            {
                std::cout << "Test: FAILED [expected result to have Point(" << p.x << "," << p.y << ")]\n" << "    Description: " << description << "\n";
                return;
            }
        }
    }

    for (const LineSegment ls : actualIntersectionLineSegments)
    {
        for (auto it = actualIntersectionLineSegments.begin(); it != actualIntersectionLineSegments.end(); ++it)
        {
            if (*it == ls)
            {
                actualIntersectionLineSegments.erase(it);
                break;
            }
            else if (it + 1 == actualIntersectionLineSegments.end())
            {
                std::cout << "Test: FAILED [expected result to have LineSegment({" << ls.a.x << "," << ls.a.y << "},{" << ls.b.x << "," << ls.b.y  << "})]\n" << "    Description: " << description << "\n";
                return;
            }
        }
    }

    std::cout << "Test: PASSED\n" << "    Description: " << description << "\n";
    return;
}

void testLineSegmentHas(LineSegment ls, Point a, bool actual)
{
    bool result = ls.has(a);

    std::cout << "Result = " << result << ", Actual = " << actual << "\n";
}

void testRayHas(Ray r, Point a, bool actual)
{
    bool result = r.has(a);

    std::cout << "Result = " << result << ", Actual = " << actual << "\n";
}

void testLineHas(Line l, Point a, bool actual)
{
    bool result = l.has(a);

    std::cout << "Result = " << result << ", Actual = " << actual << "\n"; 
}

void testGetDirection(Ray r, int actual)
{
    int result = r.getDirection();

    std::cout << "Result = " << result << ", Actual = " << actual << "\n";
}

void testToRay(Point base, Point p, Ray actual)
{
    Ray result = Ray(base, p);

    std::cout << "Result = [(" << result.base.x << "," << result.base.y << "), " << result.angle << "], Actual = [(" << actual.base.x << "," << actual.base.y << "), " << actual.angle << "]\n";
}

void testLineIntersection(LineSegment l1, LineSegment l2, Point actual)
{
    Point result = l1.toLine().intersection(l2.toLine());

    std::cout << "Result = (" << result.x << "," << result.y << "), Actual = (" << actual.x << "," << actual.y << ")\n";
}

void testIntersectionCount(LineSegment a, LineSegment b, int actual)
{
    int result = a.toLine().intersectionCount(b.toLine());

    std::cout << "Result = " << (result == ZERO ? "ZERO" : result == MANY ? "MANY" : "ONE") << ", Actual = " << (actual == ZERO ? "ZERO" : actual == MANY ? "MANY" : "ONE") << "\n";
}

void testIntersectionCount(Line a, Line b, int actual)
{
    int result = a.intersectionCount(b);

    std::cout << "Result = " << (result == ZERO ? "ZERO" : result == MANY ? "MANY" : "ONE") << ", Actual = " << (actual == ZERO ? "ZERO" : actual == MANY ? "MANY" : "ONE") << "\n";
}

int main(int argc, char* argv[]) 
{
    std::cout << "Tests: intersection()\n";
    testLineIntersection(LineSegment(Point(0,0), Point(10,10)), LineSegment(Point(10,0), Point(0,10)), Point(5,5));
    testLineIntersection(LineSegment(Point(4,0), Point(4,100)), LineSegment(Point(0,0), Point(8,8)), Point(4,4));
    testLineIntersection(LineSegment(Point(0,100), Point(0,0)), LineSegment(Point(-10,7.5),Point(11,7.5)), Point(0,7.5));
    testLineIntersection(LineSegment(Point(0,0), Point(103.23,0)), LineSegment(Point(23,1.5), Point(29,-1.5)), Point(26,0));
    testLineIntersection(LineSegment(Point(15,14), Point(11,-4)), LineSegment(Point(0,0), Point(26,10)), Point(13,5));

    std::cout << "Tests: intersectionCount()\n";
    testIntersectionCount(LineSegment(Point(0,0), Point(2,4)), LineSegment(Point(5,0), Point(7,4)), ZERO);
    testIntersectionCount(LineSegment(Point(0,0), Point(10,0)), LineSegment(Point(5,-5), Point(5,5)), ONE);
    testIntersectionCount(LineSegment(Point(-7,-7), Point(0,0)), LineSegment(Point(0,0), Point(7,7)), MANY);
    testIntersectionCount(Line(PI, Point(0,0)), Line(0, Point(0,0)), MANY);

    std::cout << "Test: toRay()\n";
    testToRay(Point(5,7), Point(5,21), Ray(PI/2, Point(5,7)));
    testToRay(Point(-12,-13), Point(-12,-101), Ray(-PI/2, Point(-12,-13)));
    testToRay(Point(7,10), Point(-13,10), Ray(PI,Point(7,10)));
    testToRay(Point(0,0), Point(1000,0), Ray(0, Point(0,0)));
    testToRay(Point(0,5), Point(7,12), Ray(PI/4, Point(0,5)));

    std::cout << "Test: getDirection()\n";
    testGetDirection(Ray(PI, Point(0,0)), WEST);
    testGetDirection(Ray(-PI/2, Point(0,0)), SOUTH);
    testGetDirection(Ray(PI/4, Point(0,0)), NORTH_EAST);
    testGetDirection(Ray(-PI/4 - 4 * PI, Point(0,0)), SOUTH_EAST);
    testGetDirection(Ray(PI/4 + 2 * PI, Point(0,0)), NORTH_EAST);

    std::cout << "Test: Line.has()\n";
    testLineHas(LineSegment(Point(1,1),Point(2,2)).toLine(), Point(6,6), true);
    testLineHas(LineSegment(Point(1,1),Point(2,2)).toLine(), Point(1,-12), false);
    testLineHas(LineSegment(Point(0,0),Point(0,10)).toLine(), Point(1,1), false);
    testLineHas(LineSegment(Point(0,0),Point(0,10)).toLine(), Point(0,123), true);
    testLineHas(LineSegment(Point(0,0), Point(-10,-10)).toLine(), Point(-10,-10), true);
    
    std::cout << "Test: Ray.has()\n";
    // ray but point is not on line representation or ray
    testRayHas(Ray(Point(0,0), Point(4,2)), Point(4,-1098), false);
    // Rest of tests have a point that is on the line representation of the ray.
    // ray pointing up with point
    testRayHas(Ray(Point(0,0), Point(0,10)), Point(0,4), true);
    // ray point down with point
    testRayHas(Ray(Point(0,0), Point(0,-10)), Point(0,-1000.25), true);
    // ray point right with point
    testRayHas(Ray(Point(0,0), Point(1,1)), Point(12,12), true);
    // ray point left with point
    testRayHas(Ray(Point(0,0), Point(-1,1)), Point(-2,2), true);
    // ray pointing up without point
    testRayHas(Ray(Point(0,0), Point(0,10)), Point(0,-4), false);
    // ray point down without point
    testRayHas(Ray(Point(0,0), Point(0,-10)), Point(0,1000.25), false);
    // ray point right without point
    testRayHas(Ray(Point(0,0), Point(1,1)), Point(-12,-12), false);
    // ray point left without point
    testRayHas(Ray(Point(0,0), Point(-1,1)), Point(2,-2), false);
    // FUCK
    testRayHas(Ray(Point(0,0), Point(-10,-10)), Point(-10,-10), true);

    std::cout << "Test: LineSegment.has()\n";
    // a point that is on ls
    testLineSegmentHas(LineSegment(Point(1,1), Point(10,10)), Point(9,9), true);
    // a point that is on line, but not ls
    testLineSegmentHas(LineSegment(Point(-1,-1), Point(-9,-9)), Point(-10,-10), false);
    // a point that is not on line or ls
    testLineSegmentHas(LineSegment(Point(2,4), Point(43,301)), Point(12,-98), false);

    std::cout << "Test: getIntersections()\n";
    {
        // Test 1: 4 line segments with 2 intersections points

        const Ray r = Ray(Point(0,0), Point(0,21));
        std::vector<LineSegment> ls;

        ls.push_back(LineSegment(Point(-121,-98), Point(78,-73))); // ls1
        ls.push_back(LineSegment(Point(-3,-2), Point(7,8))); // ls2
        ls.push_back(LineSegment(Point(-11,21), Point(23,21))); // ls3
        ls.push_back(LineSegment(Point(41,27), Point(2,37))); // ls4

        std::vector<Point> actualIntersectionPoints;

        actualIntersectionPoints.push_back(Point(0,1)); // i1
        actualIntersectionPoints.push_back(Point(0,21)); // i2

        std::vector<LineSegment> actualIntersectionLineSegments;

        testGetIntersections(r, ls, actualIntersectionPoints, actualIntersectionLineSegments, "4 line segments, with 2 intersection points");
    }
    {
        // Test 2: 1 line segment intersection, 1 intersection point, and 3 not intersected line segments

        const Ray r = Ray(Point(0,0), Point(8,0));
        std::vector<LineSegment> ls;

        ls.push_back(LineSegment(Point(-100,0), Point(-7,0))); // ls1
        ls.push_back(LineSegment(Point(4,-5), Point(12,-25))); // ls2
        ls.push_back(LineSegment(Point(4,10), Point(12,10))); // ls3
        ls.push_back(LineSegment(Point(8,0), Point(24,0))); // ls4
        ls.push_back(LineSegment(Point(32,5), Point(32,-5))); // ls5

        std::vector<Point> actualIntersectionPoints;

        actualIntersectionPoints.push_back(Point(32,0)); // i1

        std::vector<LineSegment> actualIntersectionLineSegments;

        actualIntersectionLineSegments.push_back(LineSegment(Point(8,0), Point(24,0))); // ils1

        testGetIntersections(r, ls, actualIntersectionPoints, actualIntersectionLineSegments, "1 line segment intersection, 1 intersection point, and 3 not intersected line segments");
    }
    {
        // Test 3: ray base between endpoints of line segment 

        const Ray r = Ray(Point(0,0), Point(-10,-10));
        std::vector<LineSegment> ls;

        ls.push_back(LineSegment(Point(-10,-10), Point(10,10))); // ls1

        std::vector<Point> actualIntersectionPoints;

        std::vector<LineSegment> actualIntersectionLineSegments;

        actualIntersectionLineSegments.push_back(LineSegment(Point(-10,-10), Point(0,0))); // ils1

        testGetIntersections(r, ls, actualIntersectionPoints, actualIntersectionLineSegments, "ray base between endpoints of line segment");
    }
    {
        // Test 4: no intersections

        const Ray r = Ray(Point(0,0), Point(10,10));
        std::vector<LineSegment> ls;

        ls.push_back(LineSegment(Point(-20,-100), Point(8,-100))); // ls1
        ls.push_back(LineSegment(Point(2,7), Point(2,25))); // ls2
        ls.push_back(LineSegment(Point(5,0), Point(15,10))); // ls3

        std::vector<Point> actualIntersectionPoints;

        std::vector<LineSegment> actualIntersectionLineSegments;

        testGetIntersections(r, ls, actualIntersectionPoints, actualIntersectionLineSegments, "3 line segments, no intersections");
    }
    {
        // Test 5: no duplicate intersection points

        const Ray r = Ray(Point(0,0), Point(5,0));
        std::vector<LineSegment> ls;

        ls.push_back(LineSegment(Point(5,0), Point(5,-5))); // ls1
        ls.push_back(LineSegment(Point(5,0), Point(5,5))); // ls2

        std::vector<Point> actualIntersectionPoints;

        actualIntersectionPoints.push_back(Point(5,0)); // i1

        std::vector<LineSegment> actualIntersectionLineSegments;

        testGetIntersections(r, ls, actualIntersectionPoints, actualIntersectionLineSegments, "two line segments with same endpoints which ray goes through, no duplicates intersection points");
    }

    std::cout << "Test Ray.closestPointOnRay()\n";
    {
        std::vector<Point> points;
        points.push_back(Point(5,7));
        points.push_back(Point(5,100));

        Ray r = Ray(Point(5,5), Point(5,50));

        testClosestPointOnRay(r, points, Point(5,7), "vertical ray, with 2 points to check, sorted closest to farthest");
    }
    {
        std::vector<Point> points;
        points.push_back(Point(1000,-100));
        points.push_back(Point(376, -100));
        points.push_back(Point(0,-100));

        Ray r = Ray(Point(-2,-100), Point(50,-100));

        testClosestPointOnRay(r, points, Point(0,-100), "horizontal ray, with 3 points to check, and sorted from farthest to closest");
    }
    {
        std::vector<Point> points;
        points.push_back(Point(78,78));
        points.push_back(Point(77, 77));
        points.push_back(Point(79,79));

        Ray r = Ray(Point(1,1), Point(10,10));

        testClosestPointOnRay(r, points, Point(77,77), "angled ray, with 3 points to check, and scrambled order of points");
    }
    {
        std::vector<Point> points;
        points.push_back(Point(-100,75));
        points.push_back(Point(37, -78));

        Ray r = Ray(Point(37,-78), Point(-100,75));

        testClosestPointOnRay(r, points, Point(37,-78), "angled ray, closest point is the base of ray");
    }

    return 0;
}