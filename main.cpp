#include <cmath>
#include <iostream>
#include <stdexcept>

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

struct Point
{
    float x, y;

    Point() {}
    Point(float x, float y) : x(x), y(y) {}
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

        return f(a.x) == a.y;
    }
};

struct Ray
{
    float angle; // determines direction of ray, angle is normalized [0, 2PI] (idk)
    Point base;

    Ray() {}
    Ray(float angle, Point base) : angle(angle), base(base) {}

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

    Line toLine()
    {
        float slope = (a.y - b.y) / (a.x - b.x);
        return Line(atan(slope), a);
    }
};

Line toLine(LineSegment ls)
{
    float slope = (ls.a.y - ls.b.y) / (ls.a.x - ls.b.x);
    return Line(atan(slope), ls.a);
}


Ray toRay(Point base, Point pointOnRay)
{
    if (base.x == pointOnRay.x && base.y == pointOnRay.y)
    {
        throw std::runtime_error("Ray's with no direction are not allowed!");
    }

    // Vertical rays
    if (base.x == pointOnRay.x)
    {
        // Points up
        if (pointOnRay.y > base.y)
        {
            return Ray(PI/2,base);
        }
        // Points down
        else
        {
            return Ray(-PI/2,base);
        }
    }

    float angle = atan((base.y - pointOnRay.y)/(base.x - pointOnRay.x));

    // Points left-ish
    if (pointOnRay.x < base.x)
    {
        // Angle return by atan() is wrong for rays that point left-ish.
        // Add 180 degrees to fix it.
        angle += PI;
    }

    return Ray(angle, base);
}

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

int intersectionCount(Line a, Line b)
{
    if (a.angle != b.angle)
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

// Point intersection(Ray r, LineSegment ls)
// {
//     // RULE: assume that the ray's base can never be on the line segment

//     // ASSUMPTION: assume that the ray and line segment do intersect atleast at 1 point
// }

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
    Ray result = toRay(base, p);

    std::cout << "Result = [(" << result.base.x << "," << result.base.y << "), " << result.angle << "], Actual = [(" << actual.base.x << "," << actual.base.y << "), " << actual.angle << "]\n";
}

void testIntersection(LineSegment l1, LineSegment l2, Point actual)
{
    Point result = intersection(toLine(l1), toLine(l2));

    std::cout << "Result = (" << result.x << "," << result.y << "), Actual = (" << actual.x << "," << actual.y << ")\n";
}

void testIntersectionCount(LineSegment a, LineSegment b, int actual)
{
    int result = intersectionCount(toLine(a),toLine(b));

    std::cout << "Result = " << (result == ZERO ? "ZERO" : result == MANY ? "MANY" : "ONE") << ", Actual = " << (actual == ZERO ? "ZERO" : actual == MANY ? "MANY" : "ONE") << "\n";
}

int main(int argc, char* argv[]) 
{
    std::cout << "Tests: intersection()\n";
    testIntersection(LineSegment(Point(0,0), Point(10,10)), LineSegment(Point(10,0), Point(0,10)), Point(5,5));
    testIntersection(LineSegment(Point(4,0), Point(4,100)), LineSegment(Point(0,0), Point(8,8)), Point(4,4));
    testIntersection(LineSegment(Point(0,100), Point(0,0)), LineSegment(Point(-10,7.5),Point(11,7.5)), Point(0,7.5));
    testIntersection(LineSegment(Point(0,0), Point(103.23,0)), LineSegment(Point(23,1.5), Point(29,-1.5)), Point(26,0));
    testIntersection(LineSegment(Point(15,14), Point(11,-4)), LineSegment(Point(0,0), Point(26,10)), Point(13,5));

    std::cout << "Tests: intersectionCount()\n";
    testIntersectionCount(LineSegment(Point(0,0), Point(2,4)), LineSegment(Point(5,0), Point(7,4)), ZERO);
    testIntersectionCount(LineSegment(Point(0,0), Point(10,0)), LineSegment(Point(5,-5), Point(5,5)), ONE);
    testIntersectionCount(LineSegment(Point(-7,-7), Point(0,0)), LineSegment(Point(0,0), Point(7,7)), MANY);

    std::cout << "Test: toRay()\n";
    testToRay(Point(5,7), Point(5,21), Ray(PI/2, Point(5,7)));
    testToRay(Point(-12,-13), Point(-12,-101), Ray(-PI/2, Point(-12,-13)));
    testToRay(Point(7,10), Point(-13,10), Ray(PI,Point(7,10)));
    testToRay(Point(0,0), Point(1000,0), Ray(0, Point(1000,0)));
    testToRay(Point(0,5), Point(7,12), Ray(PI/4, Point(0,5)));

    std::cout << "Test: getDirection()\n";
    testGetDirection(Ray(PI, Point(0,0)), WEST);
    testGetDirection(Ray(-PI/2, Point(0,0)), SOUTH);
    testGetDirection(Ray(PI/4, Point(0,0)), NORTH_EAST);
    testGetDirection(Ray(-PI/4 - 4 * PI, Point(0,0)), SOUTH_EAST);
    testGetDirection(Ray(PI/4 + 2 * PI, Point(0,0)), NORTH_EAST);

    std::cout << "Test: Line.has()\n";
    testLineHas(toLine(LineSegment(Point(1,1),Point(2,2))), Point(6,6), true);
    testLineHas(toLine(LineSegment(Point(1,1),Point(2,2))), Point(1,-12), false);
    testLineHas(toLine(LineSegment(Point(0,0),Point(0,10))), Point(1,1), false);
    testLineHas(toLine(LineSegment(Point(0,0),Point(0,10))), Point(0,123), true);
    
    std::cout << "Test: Ray.has()\n";
    // ray but point is not on line representation or ray
    testRayHas(toRay(Point(0,0), Point(4,2)), Point(4,-1098), false);
    // Rest of tests have a point that is on the line representation of the ray.
    // ray pointing up with point
    testRayHas(toRay(Point(0,0), Point(0,10)), Point(0,4), true);
    // ray point down with point
    testRayHas(toRay(Point(0,0), Point(0,-10)), Point(0,-1000.25), true);
    // ray point right with point
    testRayHas(toRay(Point(0,0), Point(1,1)), Point(12,12), true);
    // ray point left with point
    testRayHas(toRay(Point(0,0), Point(-1,1)), Point(-2,2), true);
    // ray pointing up without point
    testRayHas(toRay(Point(0,0), Point(0,10)), Point(0,-4), false);
    // ray point down without point
    testRayHas(toRay(Point(0,0), Point(0,-10)), Point(0,1000.25), false);
    // ray point right without point
    testRayHas(toRay(Point(0,0), Point(1,1)), Point(-12,-12), false);
    // ray point left without point
    testRayHas(toRay(Point(0,0), Point(-1,1)), Point(2,-2), false);

    std::cout << "Test: LineSegment.has()\n";
    // a point that is on ls
    testLineSegmentHas(LineSegment(Point(1,1), Point(10,10)), Point(9,9), true);
    // a point that is on line, but not ls
    testLineSegmentHas(LineSegment(Point(-1,-1), Point(-9,-9)), Point(-10,-10), false);
    // a point that is not on line or ls
    testLineSegmentHas(LineSegment(Point(2,4), Point(43,301)), Point(12,-98), false);

    return 0;
}