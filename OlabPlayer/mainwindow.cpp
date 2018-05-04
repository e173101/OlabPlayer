#include "mainwindow.h"
#include "ui_mainwindow.h"

#define FRAMEINTERVAL 50
#define FRAMEBOX_MAXNUM 10
#define FRAMEBOX_TTL (FRAMEINTERVAL*100) //time to live


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer.setInterval(FRAMEINTERVAL);
    connect(&timer,SIGNAL(timeout()),this,SLOT(refresh()));
    timer.start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refresh()
{
    QString strTotalBoxStaus;
    for(int i=0;i<frameBoxLife.size();i++)
    {
        strTotalBoxStaus+=QString::number(frameBoxLife[i]);
        strTotalBoxStaus+=' ';
        frameBoxLife[i] -= FRAMEINTERVAL;
        if(frameBoxLife[i] <= 0)
            frameBoxLife.remove(i);
    }
    qDebug() << strTotalBoxStaus;
}

void MainWindow::on_pushButton_snapshot_clicked()
{    if (frameBoxLife.size()<FRAMEBOX_MAXNUM)
    {
        frameBoxLife << FRAMEBOX_TTL;
    }

}
