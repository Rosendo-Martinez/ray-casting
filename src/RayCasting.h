#include <vector>

const float PI = 3.14159265359f;

enum class RayDirection
{
    N, E, S, W, NE, SE, NW, SW
};

enum class IntersectionCount
{
    Zero, One, Many
};

enum class LineTypes
{
    Horizontal, Angled, Vertical
};

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
    LineTypes type() const;
    bool  has(const Point a) const;
    float normalizedAngle() const;
    Point intersection(const Line other) const;
    bool  isParallel(const Line other) const;
    IntersectionCount intersectionCount(const Line other) const;
};

struct Ray
{
    float angle; // angle at which ray goes off from the positive x-axis
    Point base; // the base of the ray

    Ray();
    Ray(const float angle, const Point base);
    Ray(const Point base, const Point pointOnRay);

    RayDirection getDirection() const;
    bool  hasOverlap(const Point point) const;
    Line  toLine() const;
    Point closestPointOnRay(const std::vector<Point> & points) const;
};

struct LineSegment
{
    Point a, b; // the endpoints of the line segment

    LineSegment();
    LineSegment(const Point a, const Point b);

    bool hasOverlap(const Point point) const;
    Line toLine() const;
    bool operator==(const LineSegment& other) const;
};

// Functions to use for ray-intersection detection:

void getAllIntersectionsOfRay(const Ray r, const std::vector<LineSegment>& lineSegments, std::vector<Point>& intersectionPoints, std::vector<LineSegment>& intersectionLineSegments);

std::vector<Ray> getAllIntersectionsOfRays(const Point rayBase, const int rayCount, const std::vector<LineSegment>& lineSegments, std::vector<Point>& intersectionPoints, std::vector<LineSegment>& intersectionLineSegments);

void getClosestIntersectionsOfRays(const Point rayBase, const int rayCount, const std::vector<LineSegment>& lineSegments, std::vector<Point>& closestIntersections);

void getClosestIntersectionOfRays(const Point rayBase, const std::vector<LineSegment>& lineSegments, std::vector<Point>& closestIntersections);