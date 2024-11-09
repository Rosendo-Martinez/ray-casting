#include <cmath>
#include <algorithm>
#include <stdexcept>
#include "RayCasting.h"
#include <iostream>

bool almostEqual(float a, float b, float epsilon = 1e-5f) {
    return std::fabs(a - b) < epsilon;
}

/**
 * Creates a point with coordinates (0,0).
 */
Point::Point() : x(0), y(0) {}

/**
 * Creates a point with given coordinates.
 */
Point::Point(const float x, const float y) : x(x), y(y) {}

/**
 * Checks if two points are equal.
 * 
 * Warning: function does not take into account floating point errors!
 */
bool Point::operator==(const Point other) const
{
    return other.x == x && other.y == y;
}

/**
 * Returns the distance squared between two points.
 */
float Point::distSquared(const Point other) const
{
    return (other.x - x) * (other.x - x) + (other.y - y) * (other.y - y);
}

/**
 * Creates the default line, a horizontal line that goes through the origin.
 */
Line::Line() : angle(0), p(Point(0,0)) {}

/**
 * Creates a line with the given angle that goes through the given point.
 */
Line::Line(const float angle, const Point p) : angle(angle), p(p) {}

/**
 * Returns the x-intercept, the x-value at which the line crosses the x-axis.
 * 
 * Warning: throws error if line is horizontal.
 */
float Line::xIntercept() const
{
    if (type() == LineTypes::Horizontal)
    {
        throw std::runtime_error("Can't call xIntercept() on horizontal line.");
    }

    if (type() == LineTypes::Vertical)
    {
        return p.x;
    }

    return -(yIntercept()/slope());
}

/**
 * Returns the y-intercept, the y-value at which the line crosses the y-axis.
 * 
 * Warning: throws error if line is vertical.
 */
float Line::yIntercept() const
{
    if (type() == LineTypes::Vertical)
    {
        throw std::runtime_error("Can't call yIntercept() on vertical line.");
    }

    if (type() == LineTypes::Horizontal)
    {
        return p.y;
    }

    return p.y - slope() * p.x;
}

/**
 * Returns the y-value of the point (x,f(x)).
 * 
 * Warning: throws error if line is vertical.
 */
float Line::f(const float x) const
{
    if (type() == LineTypes::Vertical)
    {
        throw std::runtime_error("Can't call f(x) on vertical line.");
    }

    return slope() * x + yIntercept();
}

/**
 * Returns the x-value of the point (fInverse(y), y).
 * 
 * Warning: throws error if line is horizontal.
 */
float Line::fInverse(const float y) const
{
    if (type() == LineTypes::Horizontal)
    {
        throw std::runtime_error("Can't call fInverse(y) on horizontal line.");
    }

    return (y - yIntercept())/slope();
}

/**
 * Returns the slope of the line.
 * 
 * The m in the equation:
 *   y = m*x + b
 * 
 * Warning: throws error if line is vertical.
 */
float Line::slope() const
{
    if (type() == LineTypes::Vertical)
    {
        throw std::runtime_error("Can't call slope() on vertical line.");
    }

    return tan(angle);
}

/**
 * Returns type of line.
 * 
 * Types:
 *   Horizontal
 *   Angled
 *   Vertical
 */
LineTypes Line::type() const
{
    // Horizontal
    if (angle == 0.f || std::floor(angle / PI) == angle / PI)
    {
        return LineTypes::Horizontal;
    }
    // Vertical
    else if (std::floor(angle / (PI/2)) == angle / (PI/2))
    {
        return LineTypes::Vertical;
    }
    // Angled
    else
    {
        return LineTypes::Angled;
    }
}

/**
 * Checks if the line has the point.
 * 
 * Warning: this function tries to account for floating point errors,
 * however, its not perfect, so use this function only if you must, because
 * it could return unexpected results!
 */
bool Line::has(const Point a) const
{
    if (type() == LineTypes::Vertical)
    {
        return a.x == p.x;
    }

    return almostEqual(f(a.x), a.y);
}

/**
 * Returns normalized angle of line.
 */
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

/**
 * The intersection point of the two lines.
 * 
 * Warning: throws error if lines have zero or many intersections
 */
Point Line::intersection(const Line other) const
{
    if (intersectionCount(other) != IntersectionCount::One)
    {
        throw std::runtime_error("Lines have zero or many intersection points but still called intersection().");
    }

    Point inter;
    if (type() == LineTypes::Vertical && other.type() == LineTypes::Angled)
    {
        inter.x = xIntercept();
        inter.y = other.f(inter.x);
    }
    else if (type() == LineTypes::Vertical && other.type() == LineTypes::Horizontal)
    {
        inter.x = xIntercept();
        inter.y = other.yIntercept();
    }
    else if (type() == LineTypes::Horizontal && other.type() == LineTypes::Angled)
    {
        inter.y = yIntercept();
        inter.x = other.fInverse(inter.y);
    }
    else if (type() == LineTypes::Horizontal && other.type() == LineTypes::Vertical)
    {
        inter.y = yIntercept();
        inter.x = other.xIntercept();
    }
    else if (type() == LineTypes::Angled && other.type() == LineTypes::Horizontal)
    {
        inter.y = other.yIntercept();
        inter.x = fInverse(inter.y);
    }
    else if (type() == LineTypes::Angled && other.type() == LineTypes::Vertical)
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

/**
 * Checks if the two lines are parallel.
 */
bool Line::isParallel(const Line other) const
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

/**
 * Determines the number of intersections the two lines have.
 * 
 * Possible options: zero, one, or many.
 */
IntersectionCount Line::intersectionCount(const Line other) const
{
    if (!isParallel(other))
    {
        return IntersectionCount::One;
    }
    else if (type() == LineTypes::Horizontal)
    {
        return yIntercept() == other.yIntercept() ? IntersectionCount::Many : IntersectionCount::Zero;
    }
    else // vertical or angled
    {
        return xIntercept() == other.xIntercept() ? IntersectionCount::Many : IntersectionCount::Zero;
    }
}

Ray::Ray() : angle(0), base(Point(0,0)) {}

Ray::Ray(const float angle, const Point base) : angle(angle), base(base) {}

/**
 * Creates a ray with the given base, and which passes through the second point.
 * 
 * Warning: base and pointOnRay can not be equal.
 */
Ray::Ray(const Point base, const Point pointOnRay)
{
    if (base.x == pointOnRay.x && base.y == pointOnRay.y)
    {
        std::cout << "Base(" << base.x << ", " << base.y << ")\n";
        std::cout << "PointOnRay(" << pointOnRay.x << ", " << pointOnRay.y << ")\n";
        throw std::runtime_error("Ray's with no direction are not allowed!");
    }

    angle = atan2(pointOnRay.y - base.y, pointOnRay.x - base.x);
    this->base = base;
}

/**
 * The direction at which the ray goes.
 * 
 * N is positive y-axis.
 * S is negative y-axis.
 * E is positive x-axis.
 * W is negative x-axis.
 */
RayDirection Ray::getDirection() const
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


    if (normalizedAngle == 0 || normalizedAngle == 2 * PI)
    {
        return RayDirection::E;
    }
    else if (normalizedAngle == 3 * PI / 2)
    {
        return RayDirection::S;
    }
    else if (normalizedAngle == PI)
    {
        return RayDirection::W;
    }
    else if (normalizedAngle == PI/2)
    {
        return RayDirection::N;
    }
    else if (normalizedAngle > 0 && normalizedAngle < PI/2)
    {
        return RayDirection::NE;
    }
    else if (normalizedAngle > PI/2 && normalizedAngle < PI)
    {
        return RayDirection::NW;
    }
    else if (normalizedAngle > PI && normalizedAngle < 3 * PI / 2)
    {
        return RayDirection::SW;
    }
    else
    {
        return RayDirection::SE;
    }
}

/**
 * Checks if point has x- or y-overlap with ray.
 * 
 * Use to check if point that lies on line that is parallel and
 * goes through base of ray is on the ray.
 */
bool Ray::hasOverlap(const Point point) const
{   
    const RayDirection dir = getDirection();

    if (dir == RayDirection::N)
    {
        return point.y >= base.y;
    }
    else if (dir == RayDirection::E || dir == RayDirection::NE || dir == RayDirection::SE)
    {
        return point.x >= base.x;
    }
    else if (dir == RayDirection::S)
    {
        return point.y <= base.y;
    }
    else // west, north-west, south-west
    {
        return point.x <= base.x;
    }
}

/**
 * Returns a line that is parallel and goes through 
 * the base of this ray.
 */
Line Ray::toLine() const
{
    return Line(angle, base);
}

/**
 * Given a set of points that LIE on the ray,
 * returns the closest to the base.
 * 
 * Warning: due to floating point errors, does not check if 
 * the given points all lie on the ray!
 * 
 * Warning: throws error of points.size() is zero.
 */
Point Ray::closestPointOnRay(const std::vector<Point> & points) const
{
    if (points.size() == 0)
    {
        throw std::runtime_error("Points vector is size zero. This is not allowed.");
    }

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

LineSegment::LineSegment() : a(Point(0,0)), b(Point(0,0)) {}

/**
 * Creates a line segment that starts at point a and ends at point b.
 */
LineSegment::LineSegment(const Point a, const Point b) : a(a), b(b) {}

/**
 * Checks if point has x- and y-overlap with line segment.
 * 
 * Use to check if point that lies on line that is parallel and
 * goes through both endpoints of line segment is on the line segment.
 */
bool LineSegment::hasOverlap(const Point point) const
{
    float xRange[2];
    xRange[0] = a.x < b.x ? a.x : b.x;
    xRange[1] = a.x > b.x ? a.x : b.x;

    float yRange[2];
    yRange[0] = a.y < b.y ? a.y : b.y;
    yRange[1] = a.y > b.y ? a.y : b.y;

    // Check if point is on line segment (i.e has x- and y-overlap)
    return (point.x >= xRange[0] && point.x <= xRange[1]) && (point.y >= yRange[0] && point.y <= yRange[1]);
}

/**
 * Returns the line that passes parallel through the line segment.
 */
Line LineSegment::toLine() const
{
    float slope = (a.y - b.y) / (a.x - b.x);
    return Line(atan(slope), a);
}

/**
 * Checks if two line segments are parallel, and
 * have same endpoints.
 * 
 * Note: does not take into account floating point errors.
 */
bool LineSegment::operator==(const LineSegment& other) const
{
    return (other.a == a && other.b == b) || (other.a == b && other.b == a);
}

/**
 * Calculates the intersections points between the ray and each line segment.
 */
void getIntersections(const Ray r, const std::vector<LineSegment>& lineSegments, std::vector<Point>& intersectionPoints, std::vector<LineSegment>& intersectionLineSegments)
{
    for (const LineSegment& ls : lineSegments)
    {
        IntersectionCount count = r.toLine().intersectionCount(ls.toLine());

        if (count == IntersectionCount::Zero)
        {
            // Ray and line segment are parallel and do not overlap
            continue;
        }
        else if (count == IntersectionCount::One)
        {
            const Point inter = r.toLine().intersection(ls.toLine());
            if (r.hasOverlap(inter) && ls.hasOverlap(inter))
            {
                intersectionPoints.push_back(inter);
            }
        }
        else // many intersections
        {
            // ray intersects with entire line segment
            if (r.hasOverlap(ls.a) && r.hasOverlap(ls.b))
            {
                intersectionLineSegments.push_back(ls);
            }
            // ray's base is a point between the endpoints of the line segment [base, ls.a]
            else if (r.hasOverlap(ls.a))
            {
                intersectionLineSegments.push_back(LineSegment(ls.a, r.base));
            }
            // ray's base is a point between the endpoints of the line segment [base, ls.b]
            else if (r.hasOverlap(ls.b))
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

/**
 * Calculates the intersection points for each ray on each line segment.
 * 
 * Rays are cast at equally spaced angled intervals starting from angle 0 radian.
 */
std::vector<Ray> getIntersectionsOfRays(const Point rayBase, const int rayCount, const std::vector<LineSegment>& lineSegments, std::vector<Point>& intersectionPoints, std::vector<LineSegment>& intersectionLineSegments)
{
    std::vector<Ray> rays;

    if (rayCount == 0)
    {
        return rays;
    }

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

/**
 * Calculates the CLOSEST intersection point for each ray. Essentially, the triangle fan.
 * 
 * Rays are cast at equally spaced angled intervals starting from angle 0 radian.
 */
void getClosestIntersectionsOfRays(const Point rayBase, const int rayCount, const std::vector<LineSegment>& lineSegments, std::vector<Point>& closestIntersections)
{   
    if (rayCount == 0)
    {
        return;
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

        // No intersection
        if (pointIntersectionsForCurentRay.size() == 0)
        {
            continue;
        }

        closestIntersections.push_back(r.closestPointOnRay(pointIntersectionsForCurentRay));
    }
}

/**
 * Gets unique vertices from line segments.
 */
void getVertices(const std::vector<LineSegment>& lineSegments, std::vector<Point>& vertices)
{
    for (auto ls : lineSegments)
    {
        // Check if endpoint a of line segment is already in list
        bool foundPointA = false;
        for (auto v : vertices)
        {
            if (ls.a == v)
            {
                foundPointA = true;
                break;
            }
        }

        // If endpoint a is not in list yet, add it now
        if (!foundPointA)
        {
            vertices.push_back(ls.a);
        }

        // Check if endpoint b of line segment is already in list
        bool foundPointB = false;
        for (auto v : vertices)
        {
            if (ls.b == v)
            {
                foundPointB = true;
                break;
            }
        }

        // If endpoint b is not in list yet, add it now
        if (!foundPointB)
        {
            vertices.push_back(ls.b);
        }
    }
}

/**
 * Calculates the closest intersection of ray and sets it to result.
 * 
 * Returns true if intersection found, else false if no intersection was found.
 */
bool getClosestIntersection(const Ray r, const std::vector<LineSegment> & lineSegments, Point& result)
{
    std::vector<Point> intersectionPoints;
    std::vector<LineSegment> intersectionLineSegments;

    // get intersection points and intersection line segments
    getIntersections(r, lineSegments, intersectionPoints, intersectionLineSegments);

    for (auto ls : intersectionLineSegments)
    {
        // Ray intersects with entire line segment
        // Only the endpoints are needed since 
        // we are looking for closest intersection point of ray 
        intersectionPoints.push_back(ls.a);
        intersectionPoints.push_back(ls.b);
    }

    // Ray does not intersect with any line segment
    if (intersectionPoints.size() == 0)
    {
        return false;
    }

    // get closest intersection point
    result = r.closestPointOnRay(intersectionPoints);

    return true;
}

/**
 * Casts 3 rays at each vertex of line segments.
 * One directly at it, one slightly to its left, and one slightly to its right. 
 * Only the closest intersections of each ray are returned.
 * The points are sorted by angle.
 * 
 * What is returned is essentially a triangle fan.
 */
void getClosestIntersectionOfRays2(const Point rayBase, const std::vector<LineSegment>& lineSegments, std::vector<Point>& closestIntersections)
{
    std::vector<Point> vertices;

    getVertices(lineSegments, vertices);

    const float delta = 0.0001f; // radians

    for (auto v : vertices)
    {
        // Inside line segment
        if (v == rayBase)
        {
            closestIntersections.clear();
            return;
        }

        // Cast rays: one directly at v, one slightly to its left, another slightly to its right
        const Ray direct = Ray(rayBase, v);
        const Ray counterClockwise = Ray(direct.angle + delta, rayBase);
        const Ray clockwise = Ray(direct.angle - delta, rayBase);

        // closest intersection points of rays
        Point intDirect;
        Point intCounterClockwise;
        Point intClockwise;

        // Direct ray at v
        if (getClosestIntersection(direct, lineSegments, intDirect))
        {
            // Very important code: due to floating point errors, ray cast directly at v may not actual go through v.
            // This code fixes this problem!
            if (rayBase.distSquared(v) < rayBase.distSquared(intDirect))
            {
                closestIntersections.push_back(v);
            }
            else
            {
                closestIntersections.push_back(intDirect);
            }
        }
        // Ray cast slightly to v's left
        if (getClosestIntersection(counterClockwise, lineSegments, intCounterClockwise))
        {
            closestIntersections.push_back(intCounterClockwise);
        }
        // Ray cast slightly to v's right
        if (getClosestIntersection(clockwise, lineSegments, intClockwise))
        {
            closestIntersections.push_back(intClockwise);
        }

        // Inside line segment
        if ((intDirect == rayBase) || (intCounterClockwise == rayBase) || (intClockwise == rayBase))
        {
            closestIntersections.clear();
            return;
        }
    }

    // Sort points by angle to create triangle fan
    std::sort(closestIntersections.begin(), closestIntersections.end(), [&](Point &a, Point& b)
    {
        return Ray(rayBase, a).toLine().normalizedAngle() > Ray(rayBase, b).toLine().normalizedAngle();
    });
}