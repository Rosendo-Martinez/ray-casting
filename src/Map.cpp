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
