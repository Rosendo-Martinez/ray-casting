#include "Map.h"

Map::Map() {}

/**
 * Adds a line segments.
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

/**
 * Moves a endpoint of a line segment.
 * 
 * Returns true if endpoint found, else false.
 */
bool Map::moveEndPoint(Point oldP, Point newP)
{
    bool foundAEndpoint = false;
    for (auto it = lineSegments.begin(); it != lineSegments.end(); it++)
    {
        if ((*it).a == oldP)
        {
            (*it).a = newP;
            foundAEndpoint = true;
        }

        if ((*it).b == oldP)
        {
            (*it).b = newP;
            foundAEndpoint = true;
        }
    }

    return foundAEndpoint;
}

/**
 * Figures out the closest endpoint to p.
 * 
 * Returns true if found a endpoint within the maxDist, else false and result
 * value is meaningless.
 */
bool Map::closestEndPoint(Point p, float maxDist, Point & result)
{
    const float maxDistSquared = maxDist * maxDist;

    Point closestEndPoint;
    bool foundEndPointWithinMaxDist = false;
    
    for (auto it = lineSegments.begin(); it != lineSegments.end(); it++)
    {
        if ((*it).a.distSquared(p) < maxDistSquared)
        {
            if (foundEndPointWithinMaxDist)
            {
                if ((*it).a.distSquared(p) < closestEndPoint.distSquared(p))
                {
                    closestEndPoint = (*it).a;
                }
            }
            else
            {
                closestEndPoint = (*it).a;
                foundEndPointWithinMaxDist = true;
            }
        }
        if ((*it).b.distSquared(p) < maxDistSquared)
        {
            if (foundEndPointWithinMaxDist)
            {
                if ((*it).b.distSquared(p) < closestEndPoint.distSquared(p))
                {
                    closestEndPoint = (*it).b;
                }
            }
            else
            {
                closestEndPoint = (*it).b;
                foundEndPointWithinMaxDist = true;
            }
        }
    }

    if (foundEndPointWithinMaxDist)
    {
        result = closestEndPoint;
        return true;
    }

    return false;
}

/**
 * Gets the line segments.
 */
const std::vector<LineSegment>& Map::getLineSegments()
{
    return lineSegments;
}