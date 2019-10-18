#include "openglpanel.h"

OpenGLPanel::OpenGLPanel(QWidget *parent) : QOpenGLWidget {parent}
{

}

void OpenGLPanel::initializeGL(){
    initializeOpenGLFunctions();
    glClearColor(1, 1, 1, 1);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho(0, 40*multiplier, 0, 25*multiplier, 0, -1);
    glMatrixMode( GL_MODELVIEW );
}

void OpenGLPanel::addPoint(Point ponto){
    pontos.push_back(ponto);
    this->visualizePoint = Point(-1,-1);
    update();
}

void OpenGLPanel::setVisualizePoint(Point ponto){
    this->visualizePoint = ponto;
    update();
}

void OpenGLPanel::limparTudo(){
    this->visualizePoint = Point(-1,-1);
    pontos.clear();
    update();
}


void OpenGLPanel::setCor(float r, float g, float b){
    this->r = r;
    this->g = g;
    this->b = b;
    update();
}

void OpenGLPanel::setRandomColor(){
    this->r = ((float) rand() / (RAND_MAX));
    this->g = ((float) rand() / (RAND_MAX));
    this->b = ((float) rand() / (RAND_MAX));
    printf("r: %f, g: %f, b: %f\n", r, g, b);
    update();
}

void OpenGLPanel::start()
{
    int i;

    std::vector<Point> _pontos = pontos;

    if(_pontos.size()<=0) return;

    _pontos.push_back(_pontos.at(0));

    for(i = 0;i<_pontos.size();i++)
    {
        int x = _pontos[i].x;
        _pontos.at(i).x = _pontos.at(i).x * multiplier;
        _pontos.at(i).y = _pontos.at(i).y * multiplier;
    }

    EdgeTable.clear();


    glColor3f(r,g,b);

    for(i=0; i < _pontos.size() - 1 ; i++)
    {
        glBegin(GL_LINES);
            glVertex2i( _pontos[i].x, _pontos[i].y);
            glVertex2i( _pontos[i+1].x, _pontos[i+1].y);
        glEnd();
        if(!DEBUG) addEdgeToET(_pontos[i].x, _pontos[i].y, _pontos[i+1].x, _pontos[i+1].y);
        else addEdgeToET(_pontos[i].x, _pontos[i].y, _pontos[i+1].x, _pontos[i+1].y, _pontos[i].name, _pontos[i+1].name);
    }

    ScanLine(25*multiplier);
    std::thread timer();
}

void OpenGLPanel::addEdgeToET(int x1, int y1, int x2, int y2, char from, char to)
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

        int count = 0, x1 = 0, x2 =0, ymax1 = 0, ymax2 = 0;
        bool toFill = false;

        for(EdgeBucket bucket : ActiveEdgeTable.buckets)
        {
            if(DEBUG) printf("Bucket: (%d,%lf,%lf) (%c%c) at coordCount = %d\n",
                             bucket.ymax, bucket.xmin, bucket.m, bucket.from, bucket.to ,count);
            if(count % 2 == 0){
                x1 = (int)bucket.xmin;
                ymax1 = bucket.ymax;
                if(x1 == x2)
                {
                    if (((x1==ymax1)&&(x2!=ymax2))||((x1!=ymax1)&&(x2==ymax2))){
                        if(DEBUG) printf("Intersecção!!\n");
                        x2 = x1;
                        ymax2 = ymax1;
                    }
                    else count++;
                }
                else count++;
            }
            else{
                x2 = (int) bucket.xmin;
                ymax2 = bucket.ymax;
                toFill = false;
                if(x1 == x2){
                     if(((x1==ymax1)&&(x2!=ymax2))||((x1!=ymax1)&&(x2==ymax2))){
                         if(DEBUG) printf("Intersecção!!\n");
                         x1 = x2;
                         ymax1 = ymax2;
                     }
                     else{
                         count++;
                         toFill = true;
                         if(DEBUG) printf("Setted FillFlag\n");
                     }
                }
                else{
                    count++;
                    toFill = true;
                    if(DEBUG) printf("Setted FillFlag\n");
                }
                if(toFill)
                {
                    if(DEBUG) printf("Filling line: %d from %d to %d\n", scanline, x1, x2);
                    glColor3f(r,g,b);
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

void OpenGLPanel::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT);

    start();


    if(visualizePoint.x >= 0 && visualizePoint.y >= 0)
    {
        if(r == 0.0 && g == 0.0 && b == 0.0) glColor3f(1,0,0);
        else glColor3f(0,0,0);
        glPointSize(10);
        glBegin(GL_POINTS);
            glVertex2i(visualizePoint.x*multiplier, visualizePoint.y*multiplier);
        glEnd();
        glFlush();
    }
}



