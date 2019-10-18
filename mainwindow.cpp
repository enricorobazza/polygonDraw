#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    OpenGLPanel *panel = findViewByID(ui->centralwidget, "openglpanel");

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
            Point(1,9,'J')
        };

    connect(this, &MainWindow::sendVisualizePoint, panel, &OpenGLPanel::setVisualizePoint);
    connect(this, &MainWindow::sendAddPoint, panel, &OpenGLPanel::addPoint);
    connect(this, &MainWindow::sendLimparTudo, panel, &OpenGLPanel::limparTudo);
    connect(this, &MainWindow::sendCor, panel, &OpenGLPanel::setCor);
    connect(this, &MainWindow::sendRandomCor, panel, &OpenGLPanel::setRandomColor);
}

OpenGLPanel *MainWindow::findViewByID(QWidget *parent, char *ID)
{
    for(QObject *mychild : parent->children())
    {
        if(strcmp(ID, mychild->objectName().toStdString().c_str())==0)
        {
            return (OpenGLPanel *)mychild;
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btnVisualizar_clicked()
{
   int x = ui->txtX->text().toInt();
   int y = ui->txtY->text().toInt();

   sendVisualizePoint(Point(x,y));
}

void MainWindow::on_btnAdicionar_clicked()
{
    int x = ui->txtX->text().toInt();
    int y = ui->txtY->text().toInt();
    sendAddPoint(Point(x,y));

}

void MainWindow::on_btnLimpar_clicked()
{
    sendLimparTudo();
}

void MainWindow::on_btnCor_clicked()
{
    float r = ui->txtRed->text().toFloat();
    float g = ui->txtGreen->text().toFloat();
    float b = ui->txtBlue->text().toFloat();

    sendCor(r, g, b);
}

void MainWindow::on_btnColors_clicked()
{
    sendRandomCor();
}
