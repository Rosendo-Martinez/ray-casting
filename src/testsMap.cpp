#include "Map.h"
#include <iostream>


int main(int argc, char* argv[]) 
{
    {
        Map m;
        
        if (m.addPoint(Point(0,0)) != true)
        {
            std::cout<< "Test 1: FAILED - failed to add point to empty map\n";
        }
        else
        {
            std::cout<< "Test 1: Passed\n";
        }
    }
    {
        Map m;

        m.addPoint(Point(5,10));
        if (m.addPoint(Point(20,20)) != true)
        {
            std::cout << "Test 2: FAILED - failed to add point to non-empty map\n";
        }
        else
        {
            std::cout<< "Test 2: Passed\n";
        }
    }
    {
        Map m;

        Point p = Point(5,50);
        m.addPoint(p);
        if (m.addPoint(p) != false)
        {
            std::cout << "Test 3: FAILED - added existing point\n";
        }
        else
        {
            std::cout << "Test 3: Passed\n";
        }
    }

    {
        Map m;
        
        if (m.addLineSegment(LineSegment(Point(0,0), Point(10,10))) != true)
        {
            std::cout<< "Test 4: FAILED - failed to line segment to empty map\n";
        }
        else
        {
            std::cout<< "Test 4: Passed\n";
        }
    }
    {
        Map m;

        m.addLineSegment(LineSegment(Point(0,0), Point(10,10)));
        if (m.addLineSegment(LineSegment(Point(20,20), Point(40,40))) != true)
        {
            std::cout << "Test 5: FAILED - failed to add line segment to non-empty map\n";
        }
        else
        {
            std::cout<< "Test 5: Passed\n";
        }
    }
    {
        Map m;

        LineSegment ls = LineSegment(Point(0,0), Point(10,10));
        m.addLineSegment(ls);
        if (m.addLineSegment(ls) != false)
        {
            std::cout << "Test 6: FAILED - added existing line segment\n";
        }
        else
        {
            std::cout << "Test 6: Passed\n";
        }
    }
    
    return 0;
}