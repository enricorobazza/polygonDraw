#ifndef TABLE_H
#define TABLE_H

#include "edgebucket.h"
#include <iostream>
#include <vector>

class Table
{
private:
    static bool compareBuckets(EdgeBucket b1, EdgeBucket b2);
    void sort();
public:
    Table();
    std::vector<EdgeBucket> buckets;
    void addBucket(int ymax, int xmin, double m, char from, char to);
    void addBucket(int ymax, int xmin, double m);
    void addBucket(EdgeBucket bucket);
    void removeBucketsAtYMax(int scanline);
    void updateX();
};

#endif // TABLE_H
