#include "Map.h"

Map::Map() {}

/**
 * Adds a point.
 * 
 * Returns true if added point, else false if point has already been added.
 */
bool Map::addPoint(Point p)
{
    for (auto x : points)
    {
        if (p == x)
        {
            return false;
        }
    }

    points.push_back(p);
    return true;
}

/**
 * Adds a line segments, and adds the endpoints as points.
 * 
 * Returns true if added line segment, else false if line segment has already been added.
 */
bool Map::addLineSegment(LineSegment ls)
{
    for (auto x : lineSegments)
    {
        if (ls == x)
        {
            return false;
        }
    }

    lineSegments.push_back(ls);
    addPoint(ls.a);
    addPoint(ls.b);

    return true;
}

int Map::sizePoints()
{
    return points.size();
}

int Map::sizeLineSegments()
{
    return lineSegments.size();
}

/**
 * Removes the pont p.
 * 
 * Return true if point removed, else false if point not found.
 */
bool Map::removePoint(Point p)
{
    for (auto it = points.begin(); it != points.end(); it++)
    {
        if (*it == p)
        {
            points.erase(it);
            return true;
        }
    }

    return false;
}

/**
 * Removes line segment.
 * 
 * Does not remove endpoints.
 * 
 * Returns true if line segment removed, else false if not found.
 */
bool Map::removeLineSegment(LineSegment ls)
{
    for (auto it = lineSegments.begin(); it != lineSegments.end(); it++)
    {
        if (*it == ls)
        {
            lineSegments.erase(it);
            return true;
        }
    }

    return false;
}

/**
 * Moves a point.
 * 
 * If point is endpoint of line segment,
 * then the endpoint will also move.
 * 
 * Returns true if point moved, else false if point not found.
 */
bool Map::movePoint(Point p)
{
    
}