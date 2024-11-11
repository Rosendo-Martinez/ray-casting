#include "RayCasting.h"
#include <vector>

class Map
{
private:
    std::vector<Point> points;
    std::vector<LineSegment> lineSegments;

public:

    Map();

    bool addPoint(Point p);
    bool addLineSegment(LineSegment ls);
    int sizePoints();
    int sizeLineSegments();
};