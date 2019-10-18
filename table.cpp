#include "table.h"

bool DEBUG = false;

Table::Table()
{

}

bool Table::compareBuckets(EdgeBucket b1, EdgeBucket b2){
    return b1.xmin < b2.xmin;
}

void Table::sort(){
    std::sort(this->buckets.begin(), this->buckets.end(), this->compareBuckets);
}

void Table::addBucket(int ymax, int xmin, double m, char from, char to)
{
    EdgeBucket bucket = EdgeBucket(ymax, xmin, m);
    bucket.from = from;
    bucket.to = to;
    buckets.push_back(bucket);
    this->sort();
}

void Table::addBucket(int ymax, int xmin, double m){
    buckets.push_back(EdgeBucket(ymax, xmin, m));
    this->sort();
}

void Table::addBucket(EdgeBucket bucket){
    buckets.push_back(bucket);
    this->sort();
}

void Table::removeBucketsAtYMax(int scanline)
{
    for(int i =0; i<buckets.size();i++){
        if(buckets.at(i).ymax == scanline){
            if(DEBUG)
                printf("Removed bucket from AET: (%d, %lf, %lf) (%c%c)\n",
                       buckets.at(i).ymax, buckets.at(i).xmin, buckets.at(i).m, buckets.at(i).from, buckets.at(i).to);
            buckets.erase(buckets.begin() + i);
            i--;
        }
    }
    this->sort();
}

void Table::updateX(){
    for(int i = 0; i< buckets.size();i++){
        if(DEBUG)
            printf("Updated Xmin from (%d,%lf,%lf) (%c%c) to %lf\n",
                   buckets[i].ymax, buckets[i].xmin, buckets[i].m,buckets[i].from, buckets[i].to, buckets[i].xmin + buckets[i].m);
        buckets[i].xmin += buckets[i].m;
    }
}
