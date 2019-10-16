#include "openglpanel.h"

const double line_height = 0.001;
const int multiplier = 25;
const bool DEBUG = false;
int executed = 0;

class Point{
    public:
        Point(int x, int y){
            this->x = x;
            this->y = y;
        }
        Point(int x, int y, char name){
            this->x = x;
            this->y = y;
            this->name = name;
        }
        int x, y;
        char name;
};

class EdgeBucket{
public:
    EdgeBucket(int ymax, int xmin, double m){
        this->ymax = ymax;
        this->xmin = xmin;
        this->m = m;
    }
    int ymax;
    double m, xmin;
    char from, to; // Only for Debugging purposes
};

class Table{
    private:
        static bool compareBuckets(EdgeBucket b1, EdgeBucket b2){
            return b1.xmin < b2.xmin;
        }
        void sort(){
            std::sort(buckets.begin(), buckets.end(), this->compareBuckets);
        }
    public:
        std::vector<EdgeBucket> buckets;
        void addBucket(int ymax, int xmin, double m, char from, char to)
        {
            EdgeBucket bucket = EdgeBucket(ymax, xmin, m);
            bucket.from = from;
            bucket.to = to;
            buckets.push_back(bucket);
            this->sort();
        }
        void addBucket(int ymax, int xmin, double m){
            buckets.push_back(EdgeBucket(ymax, xmin, m));
            this->sort();
        }
        void addBucket(EdgeBucket bucket){
            buckets.push_back(bucket);
            this->sort();
        }
        void removeBucketsAtYMax(int scanline)
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
        void updateX(){
            for(int i = 0; i< buckets.size();i++){
                if(DEBUG)
                    printf("Updated Xmin from (%d,%lf,%lf) (%c%c) to %lf\n",
                           buckets[i].ymax, buckets[i].xmin, buckets[i].m,buckets[i].from, buckets[i].to, buckets[i].xmin + buckets[i].m);
                buckets[i].xmin += buckets[i].m;
            }
        }
};

std::map<double, Table> EdgeTable;
Table ActiveEdgeTable;

OpenGLPanel::OpenGLPanel(QWidget *parent) : QOpenGLWidget {parent}
{

}

void OpenGLPanel::initializeGL(){
    initializeOpenGLFunctions();
    glClearColor(1, 1, 0, 1);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho(0, 40*multiplier, 0, 25*multiplier, 0, -1);
    glMatrixMode( GL_MODELVIEW );
}

void addEdgeToET(int x1, int y1, int x2, int y2, char from = ';', char to = ';')
{
    int ymax, ymin, xmin;
    double m;

    if(y2 == y1) return;

    if(x2 == x1) m = 0;
    else m = ((double)(x2-x1))/((double)(y2-y1));

    if(y1 > y2){
        ymax = y1;
        ymin = y2;
        xmin = x2;
    }
    else{
        ymax = y2;
        ymin = y1;
        xmin = x1;
    }

    EdgeTable[ymin].addBucket(ymax, xmin, m, from, to);

//    EdgeTable.at(EdgeTable.size()-1).addBucket(ymax, xmin, m);
//    EdgeTable[ymin].addBucket(ymax, xmin, m);
}

void OpenGLPanel::ScanLine(int maxy){
    if(DEBUG) printf("Starting ScanLine\n");
    ActiveEdgeTable = Table();
    for(double scanline = 0; scanline < maxy; scanline+=1){

        if(DEBUG) printf("Scanline: %lf\n", scanline);
        if(EdgeTable.find(scanline) != EdgeTable.end())
        for(int i=0; i<EdgeTable.at(scanline).buckets.size(); i++)
        {
            EdgeBucket bucket = EdgeTable.at(scanline).buckets.at(i);
            ActiveEdgeTable.addBucket(bucket);
            if(DEBUG) printf("Added bucket (%d, %d, %lf) (%c%c) into AET\n", bucket.ymax, bucket.xmin, bucket.m, bucket.from, bucket.to);
        }

        ActiveEdgeTable.removeBucketsAtYMax(scanline);

        int coordCount = 0, x1 = 0, x2 =0, ymax1 = 0, ymax2 = 0, FillFlag = 0;

        for(EdgeBucket bucket : ActiveEdgeTable.buckets)
        {
            if(DEBUG) printf("Bucket: (%d,%lf,%lf) (%c%c) at coordCount = %d\n",
                             bucket.ymax, bucket.xmin, bucket.m, bucket.from, bucket.to ,coordCount);
            if(coordCount % 2 == 0){
                x1 = (int)bucket.xmin;
                ymax1 = bucket.ymax;
                if(x1 == x2)
                {
                    if (((x1==ymax1)&&(x2!=ymax2))||((x1!=ymax1)&&(x2==ymax2))){
                        if(DEBUG) printf("Intersecção!!\n");
                        x2 = x1;
                        ymax2 = ymax1;
                    }
                    else coordCount++;
                }
                else coordCount++;
            }
            else{
                x2 = (int) bucket.xmin;
                ymax2 = bucket.ymax;
                FillFlag = 0;
                if(x1 == x2){
                     if(((x1==ymax1)&&(x2!=ymax2))||((x1!=ymax1)&&(x2==ymax2))){
                         if(DEBUG) printf("Intersecção!!\n");
                         x1 = x2;
                         ymax1 = ymax2;
                     }
                     else{
                         coordCount++;
                         FillFlag = 1;
                         if(DEBUG) printf("Setted FillFlag\n");
                     }
                }
                else{
                    coordCount++;
                    FillFlag = 1;
                    if(DEBUG) printf("Setted FillFlag\n");
                }
                if(FillFlag)
                {
                    if(DEBUG) printf("Filling line: %d from %d to %d\n", scanline, x1, x2);
                    glColor3f(1,0,0);
                    glBegin(GL_LINES);
                        glVertex2i(x1,scanline);
                        glVertex2i(x2,scanline);
                    glEnd();
                    glFlush();
                }
            }
        }
        ActiveEdgeTable.updateX();
    }
}

void OpenGLPanel::start()
{
    /*std::vector<Point> pontos = {
        Point(2,3, 'A'),
        Point(7,1, 'B'),
        Point(13, 5, 'C'),
        Point(13, 11, 'D'),
        Point(7,7, 'E'),
        Point(2,9, 'F')
    };*/

    std::vector<Point> pontos={
        Point(4, 2, 'A'),
        Point(6,2,'B'),
        Point(6,6, 'C'),
        Point(9,6,'D'),
        Point(11,8,'E'),
        Point(8,11, 'F'),
        Point(4,11,'G'),
        Point(4,9,'H'),
        Point(1,9,'I'),
        Point(1,4,'J')
    };

    int i;

    pontos.push_back(pontos.at(0));

    for(i = 0;i<pontos.size();i++)
    {
        pontos[i].x = pontos[i].x * multiplier;
        pontos[i].y = pontos[i].y * multiplier;
    }

    EdgeTable.clear();

    for(i=0; i < pontos.size() - 1 ; i++)
    {
        glBegin(GL_LINES);
            glVertex2i( pontos[i].x, pontos[i].y);
            glVertex2i( pontos[i+1].x, pontos[i+1].y);
        glEnd();
        if(!DEBUG) addEdgeToET(pontos[i].x, pontos[i].y, pontos[i+1].x, pontos[i+1].y);
        else addEdgeToET(pontos[i].x, pontos[i].y, pontos[i+1].x, pontos[i+1].y, pontos[i].name, pontos[i+1].name);
    }

    ScanLine(12*multiplier);

//    std::map<int, Table>::iterator it = EdgeTable.begin();

//    while(it != EdgeTable.end())
//    {
//        for(int i=0; i < it->second.buckets.size(); i++)
//        {
//            EdgeBucket bucket = it->second.buckets.at(i);
//            printf("Bucket %d from Scanline(%d): Ymax(%d) Xmin(%d) m(%lf)\n", i, it->first ,bucket.ymax, bucket.xmin, bucket.m);
//        }
//        it++;
//    }
}

void OpenGLPanel::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT);
    start();



//    for(edgetable : EdgeTable){
//        for(int i=0;i<edgetable.buckets.size();i++){
//            EdgeBucket bucket = edgetable.buckets.at(i);
//            printf("Bucket %d: Ymax(%d) Xmin(%d) m(%lf)\n", i, bucket.ymax, bucket.xmin, bucket.m);
//        }
//    }

}

//void OpenGLPanel::resizeGL(int w, int h){

//}

