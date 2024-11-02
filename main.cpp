#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>

const float PI = 3.14159265359f;

const int HORIZONTAL = -1;
const int ANGLED = 0;
const int VERTICAL = 1;

const int ZERO = 0;
const int ONE = 1;
const int MANY = 2;

const int NORTH = 0;
const int EAST = 1;
const int SOUTH = 2;
const int WEST = 3;
const int NORTH_EAST = 4;
const int SOUTH_EAST = 5;
const int NORTH_WEST = 6;
const int SOUTH_WEST = 7;

bool almostEqual(float a, float b, float epsilon = 1e-5f) {
    return std::fabs(a - b) < epsilon;
}

struct Point
{
    float x, y;

    Point() {}
    Point(float x, float y) : x(x), y(y) {}

    bool operator==(const Point& other) const
    {
        return other.x == x && other.y == y;
    }
};

struct Line
{
    float angle; // radian
    Point p; // some point on the line

    Line() {}
    Line(float angle, Point p) : angle(angle), p(p) {}

    float xIntercept()
    {
        if (type() == HORIZONTAL)
        {
            throw std::runtime_error("Can't call xIntercept() on horizontal line.");
        }

        if (type() == VERTICAL)
        {
            return p.x;
        }

        return -(yIntercept()/slope());
    }

    float yIntercept()
    {
        if (type() == VERTICAL)
        {
            throw std::runtime_error("Can't call yIntercept() on vertical line.");
        }

        if (type() == HORIZONTAL)
        {
            return p.y;
        }

        return p.y - slope() * p.x;
    }

    float f(float x)
    {
        if (type() == VERTICAL)
        {
            throw std::runtime_error("Can't call f(x) on vertical line.");
        }

        return slope() * x + yIntercept();
    }

    float fInverse(float y)
    {
        if (type() == HORIZONTAL)
        {
            throw std::runtime_error("Can't call fInverse(y) on horizontal line.");
        }

        return (y - yIntercept())/slope();
    }

    float slope()
    {
        if (type() == VERTICAL)
        {
            throw std::runtime_error("Can't call slope() on vertical line.");
        }

        return tan(angle);
    }

    /**
     * Type of line.
     * 
     * -1 is horizontal
     *  0 is angled
     *  1 is vertical
     */
    int type()
    {
        // Horizontal
        if (angle == 0.f || std::floor(angle / PI) == angle / PI)
        {
            return HORIZONTAL;
        }
        // Vertical
        else if (std::floor(angle / (PI/2)) == angle / (PI/2))
        {
            return VERTICAL;
        }
        // Angled
        else
        {
            return ANGLED;
        }
    }

    bool has(Point a)
    {
        if (type() == VERTICAL)
        {
            return a.x == p.x;
        }

        return almostEqual(f(a.x), a.y);
    }

    float normalizedAngle() const
    {
        float normalizedAngle = angle;

        // Normalize by: abs(angle) <= 2 * PI 
        if (std::abs(normalizedAngle) > 2 * PI)
        {
            normalizedAngle = normalizedAngle - 2 * PI * std::floor(normalizedAngle/(2 * PI));
        }

        // Normalize by: angle >= 0
        if (normalizedAngle < 0)
        {
            normalizedAngle += 2 * PI;
        }

        return normalizedAngle;
    }
};

struct Ray
{
    float angle; // determines direction of ray, angle is normalized [0, 2PI] (idk)
    Point base;

    Ray() {}
    Ray(float angle, Point base) : angle(angle), base(base) {}

    Ray(Point base, Point pointOnRay)
    {
        if (base.x == pointOnRay.x && base.y == pointOnRay.y)
        {
            throw std::runtime_error("Ray's with no direction are not allowed!");
        }

        angle = atan2(pointOnRay.y - base.y, pointOnRay.x - base.x);
        this->base = base;
    }

    int getDirection()
    {
        // Idea: Maybe angle member should always be normalized?
        //       Then I wouldn't need to normalize it here.

        float normalizedAngle = angle;

        // Normalize by: abs(angle) <= 2 * PI 
        if (std::abs(normalizedAngle) > 2 * PI)
        {
            normalizedAngle = normalizedAngle - 2 * PI * std::floor(normalizedAngle/(2 * PI));
        }

        // Normalize by: angle >= 0
        if (normalizedAngle < 0)
        {
            normalizedAngle += 2 * PI;
        }


        if (normalizedAngle == 0 || normalizedAngle == 2 * PI)
        {
            return EAST;
        }
        else if (normalizedAngle == 3 * PI / 2)
        {
            return SOUTH;
        }
        else if (normalizedAngle == PI)
        {
            return WEST;
        }
        else if (normalizedAngle == PI/2)
        {
            return NORTH;
        }
        else if (normalizedAngle > 0 && normalizedAngle < PI/2)
        {
            return NORTH_EAST;
        }
        else if (normalizedAngle > PI/2 && normalizedAngle < PI)
        {
            return NORTH_WEST;
        }
        else if (normalizedAngle > PI && normalizedAngle < 3 * PI / 2)
        {
            return SOUTH_WEST;
        }
        else
        {
            return SOUTH_EAST;
        }
    }

    bool has(Point point)
    {   
        if (!Line(angle, base).has(point))
        {
            return false;
        }

        const int dir = getDirection();

        if (dir == NORTH)
        {
            return point.y >= base.y;
        }
        else if (dir == EAST || dir == NORTH_EAST || dir == SOUTH_EAST)
        {
            return point.x >= base.x;
        }
        else if (dir == SOUTH)
        {
            return point.y <= base.y;
        }
        else // west, north-west, south-west
        {
            return point.x <= base.x;
        }
    }
};


struct LineSegment
{
    Point a, b; // the endpoints of the line segment

    LineSegment() {}
    LineSegment(Point a, Point b) : a(a), b(b) {}

    bool has(Point point)
    {
        // Check if point is on line representation of line segment
        if (!toLine().has(point))
        {
            return false;
        }

        float xRange[2];
        xRange[0] = a.x < b.x ? a.x : b.x;
        xRange[1] = a.x > b.x ? a.x : b.x;

        float yRange[2];
        yRange[0] = a.y < b.y ? a.y : b.y;
        yRange[1] = a.y > b.y ? a.y : b.y;

        // Check if point is on line segment (i.e has x- and y-overlap)
        return (point.x >= xRange[0] && point.x <= xRange[1]) && (point.y >= yRange[0] && point.y <= yRange[1]);
    }

    Line toLine() const
    {
        float slope = (a.y - b.y) / (a.x - b.x);
        return Line(atan(slope), a);
    }

    bool operator==(const LineSegment& other) const
    {
        return (other.a == a && other.b == b) || (other.a == b && other.b == a);
    }
};

Line toLine(Ray r)
{
    return Line(r.angle, r.base);
}

Point intersection(Line a, Line b)
{
    // WARNING: assumes lines intersect at one and only one point

    Point inter;
    if (a.type() == VERTICAL && b.type() == ANGLED)
    {
        inter.x = a.xIntercept();
        inter.y = b.f(inter.x);
    }
    else if (a.type() == VERTICAL && b.type() == HORIZONTAL)
    {
        inter.x = a.xIntercept();
        inter.y = b.yIntercept();
    }
    else if (a.type() == HORIZONTAL && b.type() == ANGLED)
    {
        inter.y = a.yIntercept();
        inter.x = b.fInverse(inter.y);
    }
    else if (a.type() == HORIZONTAL && b.type() == VERTICAL)
    {
        inter.y = a.yIntercept();
        inter.x = b.xIntercept();
    }
    else if (a.type() == ANGLED && b.type() == HORIZONTAL)
    {
        inter.y = b.yIntercept();
        inter.x = a.fInverse(inter.y);
    }
    else if (a.type() == ANGLED && b.type() == VERTICAL)
    {
        inter.x = b.xIntercept();
        inter.y = a.f(inter.x);
    }
    else // both angled
    {
        inter.x = (a.yIntercept() - b.yIntercept()) / (b.slope() - a.slope());
        inter.y = a.f(inter.x);
    }

    return inter;
}

bool isParallel(Line a, Line b)
{
    float aNorm = a.normalizedAngle();
    float bNorm= b.normalizedAngle();

    if (aNorm > bNorm)
    {
        return aNorm == bNorm + PI;
    }
    else if (aNorm < bNorm)
    {
        return bNorm == aNorm + PI;
    }
    else // aNorm == bNorm
    {
        return true;
    }
}

int intersectionCount(Line a, Line b)
{
    if (!isParallel(a,b))
    {
        return ONE;
    }
    else if (a.type() == HORIZONTAL)
    {
        return a.yIntercept() == b.yIntercept() ? MANY : ZERO;
    }
    else // vertical or angled
    {
        return a.xIntercept() == b.xIntercept() ? MANY : ZERO;
    }
}

bool isValidIntersection(Ray r, LineSegment ls, Point inter)
{
    return r.has(inter) && ls.has(inter);
}

void getIntersections(const Ray r, const std::vector<LineSegment>& lineSegments, std::vector<Point>& intersectionPoints, std::vector<LineSegment>& intersectionLineSegments)
{
    for (const LineSegment& ls : lineSegments)
    {
        int count = intersectionCount(toLine(r), ls.toLine());

        if (count == ZERO)
        {
            // Ray and line segment are parallel and do not overlap
            continue;
        }
        else if (count == ONE)
        {
            const Point inter = intersection(toLine(r), ls.toLine());
            if (isValidIntersection(r, ls, inter))
            {
                intersectionPoints.push_back(inter);
            }
        }
        else // many intersections
        {
            // ray intersects with entire line segment
            if (isValidIntersection(r, ls, ls.a) && isValidIntersection(r, ls, ls.b))
            {
                intersectionLineSegments.push_back(ls);
            }
            // ray's base is a point between the endpoints of the line segment [base, ls.a]
            else if (isValidIntersection(r, ls, ls.a))
            {
                intersectionLineSegments.push_back(LineSegment(ls.a, r.base));
            }
            // ray's base is a point between the endpoints of the line segment [base, ls.b]
            else if (isValidIntersection(r, ls, ls.b))
            {
                intersectionLineSegments.push_back(LineSegment(ls.b, r.base));
            }
            else // ray does not intersect with line segment
            {
                continue;
            }
        }
    }

    // TO DO
    // Should I remove duplicate intersection points (not ls)? Yes.
    // I don't think that their can be duplicate ls or sub-ls's. So I don't think I'll
    // need to worry about duplicates for the lss.
}


void testGetIntersections(const Ray r, const std::vector<LineSegment>& lineSegments, std::vector<Point>& actualIntersectionPoints, std::vector<LineSegment>& actualIntersectionLineSegments, const std::string description)
{
    std::vector<Point> resultIntersectionPoints;
    std::vector<LineSegment> resultIntersectionLineSegments;

    getIntersections(r, lineSegments, resultIntersectionPoints, resultIntersectionLineSegments);


    if (resultIntersectionLineSegments.size() != actualIntersectionLineSegments.size())
    {
        std::cout << "Test: Failed [intersection line segments wrong]\n" << "    Description: " << description << "\n";

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
        std::cout << "Test: Failed [intersection points wrong]\n" << "    Description: " << description << "\n";
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
                std::cout << "Test: Failed [expected result to have Point(" << p.x << "," << p.y << ")]\n" << "    Description: " << description << "\n";
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
                std::cout << "Test: Failed [expected result to have LineSegment({" << ls.a.x << "," << ls.a.y << "},{" << ls.b.x << "," << ls.b.y  << "})]\n" << "    Description: " << description << "\n";
                return;
            }
        }
    }

    std::cout << "Test: Passed\n" << "    Description: " << description << "\n";
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
    Point result = intersection(l1.toLine(), l2.toLine());

    std::cout << "Result = (" << result.x << "," << result.y << "), Actual = (" << actual.x << "," << actual.y << ")\n";
}

void testIntersectionCount(LineSegment a, LineSegment b, int actual)
{
    int result = intersectionCount(a.toLine(),b.toLine());

    std::cout << "Result = " << (result == ZERO ? "ZERO" : result == MANY ? "MANY" : "ONE") << ", Actual = " << (actual == ZERO ? "ZERO" : actual == MANY ? "MANY" : "ONE") << "\n";
}

void testIntersectionCount(Line a, Line b, int actual)
{
    int result = intersectionCount(a,b);

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

    std::cout << "Test Ray.has()\n";


    return 0;
}