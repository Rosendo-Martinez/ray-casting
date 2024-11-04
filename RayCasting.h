#include <vector>

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

    Point();
    Point(float x, float y);

    bool  operator==(const Point& other) const;
    float distSquared(Point other) const;
};

struct Line
{
    float angle; // radian
    Point p; // some point on the line

    Line();
    Line(float angle, Point p);

    float xIntercept() const;
    float yIntercept() const;
    float f(float x) const;
    float fInverse(float y) const;
    float slope() const;
    int   type() const;
    bool  has(Point a) const;
    float normalizedAngle() const;
    Point intersection(Line other) const;
    bool  isParallel(Line other) const;
    int   intersectionCount(Line other) const;
};


struct Ray
{
    float angle; // determines direction of ray, angle is normalized [0, 2PI] (idk)
    Point base;

    Ray();
    Ray(float angle, Point base);
    Ray(Point base, Point pointOnRay);

    int   getDirection() const;
    bool  has(Point point) const;
    Line  toLine() const;
    // Point closestPoint(std::vector<Point> & points, std::vector<LineSegment> & lineSegments);
};

struct LineSegment
{
    Point a, b; // the endpoints of the line segment

    LineSegment();
    LineSegment(Point a, Point b);

    bool has(Point point) const;
    Line toLine() const;
    bool operator==(const LineSegment& other) const;
};