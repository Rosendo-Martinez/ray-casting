#include "RayCasting.h"
#include <vector>

class Map
{
private:
    std::vector<LineSegment> lineSegments;

public:

    Map();

    int  sizeLineSegments();
    bool addLineSegment(LineSegment ls);
    bool removeLineSegment(LineSegment ls);
    bool moveEndPoint(Point oldPos, Point newPos);
    bool closestEndPoint(Point p, float maxDist, Point & result);
    const std::vector<LineSegment>& getLineSegments();
};