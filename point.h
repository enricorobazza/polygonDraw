#ifndef POINT_H
#define POINT_H

class Point
{
public:
    Point(int x, int y);
    Point(int x, int y, char name);
    int x, y;
    char name;
};

#endif // POINT_H
