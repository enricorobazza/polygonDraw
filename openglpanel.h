#ifndef OPENGLPANEL_H
#define OPENGLPANEL_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QtDebug>

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
};

#endif // OPENGLPANEL_H
