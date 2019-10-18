#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "openglpanel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnVisualizar_clicked();

    void on_btnAdicionar_clicked();

    void on_btnLimpar_clicked();

    void on_btnCor_clicked();

    void on_btnColors_clicked();

signals:
    void sendVisualizePoint(Point ponto);
    void sendAddPoint(Point ponto);
    void sendLimparTudo();
    void sendCor(float r, float g, float b);
    void sendRandomCor();

private:
    Ui::MainWindow *ui;
    OpenGLPanel *findViewByID(QWidget *parent, char *ID);
    OpenGLPanel *panel;
};
#endif // MAINWINDOW_H
