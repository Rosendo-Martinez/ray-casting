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
            std::cout << "Test 2: FAILED - failed to add point to empty map\n";
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

    return 0;
}