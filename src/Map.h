#include "RayCasting.h"
#include <vector>

class Map
{
private:
    std::vector<Point> points;

public:

    Map();

    bool addPoint(Point p);
};