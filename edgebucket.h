#ifndef EDGEBUCKET_H
#define EDGEBUCKET_H


class EdgeBucket
{
public:
    EdgeBucket(int ymax, int xmin, double m);
    int ymax;
    double m, xmin;
    char from, to; // Only for Debugging purposes
};

#endif // EDGEBUCKET_H


