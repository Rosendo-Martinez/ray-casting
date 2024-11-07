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
    Point(const float x, const float y);

    bool  operator==(const Point other) const;
    float distSquared(const Point other) const;
};

struct Line
{
    float angle; // in radians
    Point p; // some point on the line

    Line();
    Line(const float angle, const Point p);

    float xIntercept() const;
    float yIntercept() const;
    float f(const float x) const;
    float fInverse(const float y) const;
    float slope() const;
    int   type() const;
    bool  has(const Point a) const;
    float normalizedAngle() const;
    Point intersection(const Line other) const;
    bool  isParallel(const Line other) const;
    int   intersectionCount(const Line other) const;
};

struct Ray
{
    float angle; // angle at which ray goes off from the positive x-axis
    Point base; // the base of the ray

    Ray();
    Ray(const float angle, const Point base);
    Ray(const Point base, const Point pointOnRay);

    int   getDirection() const;
    bool  has(const Point point) const;
    Line  toLine() const;
    Point closestPointOnRay(const std::vector<Point> & points) const;
};

struct LineSegment
{
    Point a, b; // the endpoints of the line segment

    LineSegment();
    LineSegment(const Point a, const Point b);

    bool has(const Point point) const;
    Line toLine() const;
    bool operator==(const LineSegment& other) const;
};

void getIntersections(const Ray r, const std::vector<LineSegment>& lineSegments, std::vector<Point>& intersectionPoints, std::vector<LineSegment>& intersectionLineSegments);

std::vector<Ray> getIntersectionsOfRays(Point rayBase, int rayCount, const std::vector<LineSegment>& lineSegments, std::vector<Point>& intersectionPoints, std::vector<LineSegment>& intersectionLineSegments);

void getClosestIntersectionsOfRays(Point rayBase, int rayCount, const std::vector<LineSegment>& lineSegments, std::vector<Point>& closestIntersections);