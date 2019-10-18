#ifndef OPENGLPANEL_H
#define OPENGLPANEL_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QtDebug>
#include <QMouseEvent>
#include <chrono>
#include <thread>
#include <stdlib.h>
#include "point.h"
#include "edgebucket.h"
#include "point.h"
#include "table.h"

class OpenGLPanel : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT
public:
    OpenGLPanel(QWidget *parent =  nullptr);
protected:
    void initializeGL() override;
    void paintGL() override;
    void start();
    void ScanLine(int maxy);
    void mousePressEvent(QMouseEvent *e) override;
private:
    const int multiplier = 25;
    const bool DEBUG = false;
    std::map<double, Table> EdgeTable;
    Table ActiveEdgeTable;
    void addEdgeToET(int x1, int y1, int x2, int y2, char from = ';', char to = ';');
    void timer();
    std::vector<Point> pontos;
    Point visualizePoint = Point(-1,-1);
    float r=0,g=0,b=0;
public slots:
    void addPoint(Point ponto);
    void setVisualizePoint(Point ponto);
    void limparTudo();
    void setCor(float r, float g, float b);
    void setRandomColor();
};

#endif // OPENGLPANEL_H

