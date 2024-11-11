#include "Map.h"

Map::Map() {}

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