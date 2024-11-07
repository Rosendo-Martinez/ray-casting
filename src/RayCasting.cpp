#include <cmath>
#include <algorithm>
#include <stdexcept>
#include "RayCasting.h"

bool almostEqual(float a, float b, float epsilon = 1e-5f) {
    return std::fabs(a - b) < epsilon;
}

Point::Point() {}

Point::Point(float x, float y) : x(x), y(y) {}

bool Point::operator==(const Point& other) const
{
    return other.x == x && other.y == y;
}

float Point::distSquared(Point other) const
{
    return (other.x - x) * (other.x - x) + (other.y - y) * (other.y - y);
}

Line::Line() {}

Line::Line(float angle, Point p) : angle(angle), p(p) {}

float Line::xIntercept() const
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

float Line::yIntercept() const
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

float Line::f(float x) const
{
    if (type() == VERTICAL)
    {
        throw std::runtime_error("Can't call f(x) on vertical line.");
    }

    return slope() * x + yIntercept();
}

float Line::fInverse(float y) const
{
    if (type() == HORIZONTAL)
    {
        throw std::runtime_error("Can't call fInverse(y) on horizontal line.");
    }

    return (y - yIntercept())/slope();
}

float Line::slope() const
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
int Line::type() const
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

bool Line::has(Point a) const
{
    if (type() == VERTICAL)
    {
        return a.x == p.x;
    }

    return almostEqual(f(a.x), a.y);
}

float Line::normalizedAngle() const
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

Point Line::intersection(Line other) const
{
    // WARNING: assumes lines intersect at one and only one point

    Point inter;
    if (type() == VERTICAL && other.type() == ANGLED)
    {
        inter.x = xIntercept();
        inter.y = other.f(inter.x);
    }
    else if (type() == VERTICAL && other.type() == HORIZONTAL)
    {
        inter.x = xIntercept();
        inter.y = other.yIntercept();
    }
    else if (type() == HORIZONTAL && other.type() == ANGLED)
    {
        inter.y = yIntercept();
        inter.x = other.fInverse(inter.y);
    }
    else if (type() == HORIZONTAL && other.type() == VERTICAL)
    {
        inter.y = yIntercept();
        inter.x = other.xIntercept();
    }
    else if (type() == ANGLED && other.type() == HORIZONTAL)
    {
        inter.y = other.yIntercept();
        inter.x = fInverse(inter.y);
    }
    else if (type() == ANGLED && other.type() == VERTICAL)
    {
        inter.x = other.xIntercept();
        inter.y = f(inter.x);
    }
    else // both angled
    {
        inter.x = (yIntercept() - other.yIntercept()) / (other.slope() - slope());
        inter.y = f(inter.x);
    }

    return inter;
}

bool Line::isParallel(Line other) const
{
    float thisNorm = normalizedAngle();
    float otherNorm = other.normalizedAngle();

    if (thisNorm > otherNorm)
    {
        return thisNorm == otherNorm + PI;
    }
    else if (thisNorm < otherNorm)
    {
        return otherNorm == thisNorm + PI;
    }
    else // norm == norm
    {
        return true;
    }
}

int Line::intersectionCount(Line other) const
{
    if (!isParallel(other))
    {
        return ONE;
    }
    else if (type() == HORIZONTAL)
    {
        return yIntercept() == other.yIntercept() ? MANY : ZERO;
    }
    else // vertical or angled
    {
        return xIntercept() == other.xIntercept() ? MANY : ZERO;
    }
}

Ray::Ray() {}

Ray::Ray(float angle, Point base) : angle(angle), base(base) {}

Ray::Ray(Point base, Point pointOnRay)
{
    if (base.x == pointOnRay.x && base.y == pointOnRay.y)
    {
        throw std::runtime_error("Ray's with no direction are not allowed!");
    }

    angle = atan2(pointOnRay.y - base.y, pointOnRay.x - base.x);
    this->base = base;
}

int Ray::getDirection() const
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

bool Ray::has(Point point) const
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

Line Ray::toLine() const
{
    return Line(angle, base);
}

Point Ray::closestPointOnRay(std::vector<Point> & points)
{
    if (points.size() == 0)
    {
        throw std::runtime_error("Points vector is size zero. This is not allowed.");
    }

    // Disabled due to causing floating point errors
    // for (auto p : points)
    // {
    //     if (!has(p))
    //     {
    //         throw std::runtime_error("Point must lie on ray.");
    //     }
    // }

    // WARNING: assumes that all points passed lie on ray

    Point closestSoFar = points.front();

    for (auto p : points)
    {
        if (p.distSquared(base) < closestSoFar.distSquared(base))
        {
            closestSoFar = p;
        }
    }

    return closestSoFar;
}

LineSegment::LineSegment() {}

LineSegment::LineSegment(Point a, Point b) : a(a), b(b) {}

bool LineSegment::has(Point point) const
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

Line LineSegment::toLine() const
{
    float slope = (a.y - b.y) / (a.x - b.x);
    return Line(atan(slope), a);
}

bool LineSegment::operator==(const LineSegment& other) const
{
    return (other.a == a && other.b == b) || (other.a == b && other.b == a);
}

void getIntersections(const Ray r, const std::vector<LineSegment>& lineSegments, std::vector<Point>& intersectionPoints, std::vector<LineSegment>& intersectionLineSegments)
{
    for (const LineSegment& ls : lineSegments)
    {
        int count = r.toLine().intersectionCount(ls.toLine());

        if (count == ZERO)
        {
            // Ray and line segment are parallel and do not overlap
            continue;
        }
        else if (count == ONE)
        {
            const Point inter = r.toLine().intersection(ls.toLine());
            if (r.has(inter) && ls.has(inter))
            {
                intersectionPoints.push_back(inter);
            }
        }
        else // many intersections
        {
            // ray intersects with entire line segment
            if (r.has(ls.a) && r.has(ls.b))
            {
                intersectionLineSegments.push_back(ls);
            }
            // ray's base is a point between the endpoints of the line segment [base, ls.a]
            else if (r.has(ls.a))
            {
                intersectionLineSegments.push_back(LineSegment(ls.a, r.base));
            }
            // ray's base is a point between the endpoints of the line segment [base, ls.b]
            else if (r.has(ls.b))
            {
                intersectionLineSegments.push_back(LineSegment(ls.b, r.base));
            }
            else // ray does not intersect with line segment
            {
                continue;
            }
        }
    }

    // Sort points by x first, then by y if x values are equal
    std::sort(intersectionPoints.begin(), intersectionPoints.end(), [](const Point& a, const Point& b) {
        return (a.x < b.x) || (a.x == b.x && a.y < b.y);
    });

    // Remove duplicates using std::unique, which will use operator==
    intersectionPoints.erase(std::unique(intersectionPoints.begin(), intersectionPoints.end()), intersectionPoints.end());
}

std::vector<Ray> getIntersectionsOfRays(Point rayBase, int rayCount, const std::vector<LineSegment>& lineSegments, std::vector<Point>& intersectionPoints, std::vector<LineSegment>& intersectionLineSegments)
{
    if (rayCount == 0)
    {
        throw std::runtime_error("Ray count can not be zero.");
    }

    std::vector<Ray> rays;
    float angleBetweenRays = 2 * PI / rayCount;
    for (int i = 0; i < rayCount; i++)
    {
        rays.push_back(Ray(angleBetweenRays * i, rayBase));
    }

    for (auto r : rays)
    {
        getIntersections(r, lineSegments, intersectionPoints, intersectionLineSegments);
    }

    return rays;
}

void getClosestIntersectionsOfRays(Point rayBase, int rayCount, const std::vector<LineSegment>& lineSegments, std::vector<Point>& closestIntersections)
{
    // Returns the triangle fan.
    
    if (rayCount == 0)
    {
        throw std::runtime_error("Ray count can not be zero.");
    }

    float angleBetweenRays = 2 * PI / rayCount;
    for (int i = 0; i <rayCount; i++)
    {
        Ray r = Ray(angleBetweenRays * i, rayBase);
        std::vector<Point> pointIntersectionsForCurentRay;
        std::vector<LineSegment> lineSegmentsIntersectionsForCurrentRay;
        getIntersections(r, lineSegments, pointIntersectionsForCurentRay, lineSegmentsIntersectionsForCurrentRay);

        for (auto ls : lineSegmentsIntersectionsForCurrentRay)
        {
            pointIntersectionsForCurentRay.push_back(ls.a);
            pointIntersectionsForCurentRay.push_back(ls.b);
        }

        closestIntersections.push_back(r.closestPointOnRay(pointIntersectionsForCurentRay));
    }
}