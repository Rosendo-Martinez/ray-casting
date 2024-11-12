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
    bool removePoint(Point p);
    bool removeLineSegment(LineSegment ls);
    bool movePoint(Point p);
};