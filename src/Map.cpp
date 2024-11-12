#include "Map.h"

Map::Map() {}

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

    return true;
}

/**
 * Count of line segments.
 */
int Map::sizeLineSegments()
{
    return lineSegments.size();
}

/**
 * Removes line segment.
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