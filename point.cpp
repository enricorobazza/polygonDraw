#include "point.h"

Point::Point(int x, int y)
{
    this->x = x;
    this->y = y;
}


Point::Point(int x, int y, char name)
{
    this->x = x;
    this->y = y;
    this->name = name;
}
