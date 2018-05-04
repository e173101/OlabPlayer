/*
 * mainwindow.cpp
 *
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cvmatandqimage.h"

#define FRAMEINTERVAL 50.0                  //units are ms
#define FRAMEBOX_MAXNUM 10
#define FRAMEBOX_TTL (FRAMEINTERVAL*100)    //time to live, units are ms
#define FRAMEIMG_H 160
#define FRAMEIMG_W 200
#define FRAMEINS_SHIFT QPoint(0,-80)

//#define PRINTDEBUG

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
    //if exit with frame servive
    for(int i=0;i<frameBoxLife.size();i++)
    {
        delete frameBox[i];
        delete frameImg[i];
    }
}

void MainWindow::refresh()
{
    //|0------1------2----------------45----|
    QString strTotalBoxStaus;
    for(int i=0;i<frameBoxLife.size();i++)
    {
        QPoint pos = (ui->radioButton_start->pos() + (1.0-(frameBoxLife[i]/FRAMEBOX_TTL)) * (ui->radioButton_end->pos()-ui->radioButton_start->pos()));
        frameBox[i]->move(pos);
        frameImg[i]->move(pos+FRAMEINS_SHIFT);
        strTotalBoxStaus+=QString::number(frameBoxLife[i]);
        strTotalBoxStaus+=' ';
        frameBoxLife[i] -= FRAMEINTERVAL;
        if(frameBoxLife[i] <= 0)
        {
            frameBoxLife.remove(i);
            delete frameBox[i];
            frameBox.remove(i);
            delete frameImg[i];
            frameImg.remove(i);
        }
    }
#ifdef PRINTDEBUG
    qDebug() << strTotalBoxStaus;
#endif
    //video play
    cv::Mat mat;
    if (video.isOpened())
    {
        video >> mat;
        mainVideo = QtOcv::mat2Image(mat);
        ui->label_video->setPixmap(QPixmap::fromImage(mainVideo));
    }
}

void MainWindow::on_pushButton_snapshot_clicked()
{
    if (frameBoxLife.size()<FRAMEBOX_MAXNUM)
    {
        int ind=frameBoxLife.size();
        frameBoxLife << FRAMEBOX_TTL;
        frameBox << new QLabel(QString::number(ind),this);
        frameBox[ind]->move(ui->radioButton_start->pos());
        frameBox[ind]->show();
        frameImg << new QLabel(this);
        QImage clip = mainVideo.scaled(QSize(FRAMEIMG_W,FRAMEIMG_H));
        frameImg[ind]->resize(QSize(FRAMEIMG_W,FRAMEIMG_H));
        frameImg[ind]->setPixmap(QPixmap::fromImage(clip));
//        frameImg[ind]->setFrameShape(QFrame::Panel);
//        frameImg[ind]->setFrameShadow(QFrame::Raised);
//        frameImg[ind]->setAutoFillBackground(true);
        frameImg[ind]->show();
    }
}

void MainWindow::on_pushButton_openVideo_clicked()
{
    QString videoFileName = QFileDialog::getOpenFileName(this, tr("Open a video file"));
    if (!videoFileName.isEmpty())
    {
        video = cv::VideoCapture(videoFileName.toStdString());
        if (video.isOpened())
        {
            ui->statusBar->showMessage("Open file "+videoFileName + " Success");
            refresh();
        }
        else
           ui->statusBar->showMessage(videoFileName + " is not a good VIDEO file");
    }
}
