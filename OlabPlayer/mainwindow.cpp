/*
 * mainwindow.cpp
 *
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cvmatandqimage.h"

#define FRAMEINTERVAL 1.0                  //units are ms
#define FRAMEBOX_MAXNUM 10
#define FRAMEBOX_TTL (FRAMEINTERVAL*100)    //time to live, units are ms
#define FRAMEIMG_H 160
#define FRAMEIMG_W 200
#define FRAMEINS_SHIFT QPoint(-50,-160)

#define DEFAULT_VIDEO_COLS 640
#define DEFAULT_VIDEO_ROWS 480

#define MAT_BUF_SIZE 10

//#define PRINTDEBUG

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    ui->setupUi(this);
    timer.setInterval(FRAMEINTERVAL);
    connect(&timer,SIGNAL(timeout()),this,SLOT(refresh()));
    timer.start();
    flagPlay=false;
    ui->spinBox_cols->setValue(DEFAULT_VIDEO_COLS);
    ui->spinBox_rows->setValue(DEFAULT_VIDEO_ROWS);
    ui->label_video->setMinimumSize(DEFAULT_VIDEO_COLS,DEFAULT_VIDEO_ROWS);

    maxBufSize = MAT_BUF_SIZE;
    matProThread = new MatProducer;


    //加载分类训练器，OpenCv官方文档提供的xml文档，可以直接调用
    //xml文档路径  opencv\sources\data\haarcascades

    if (!eye_cascade.load("C:OpenCV\\2.4.13\\etc\\haarcascades\\haarcascade_eye.xml"))  //需要将xml文档放在自己指定的路径下
    {
       qDebug( "Load haarcascade_eye.xml failed!" );
    }
    if (!face_cascade.load("C:OpenCV\\2.4.13\\etc\\haarcascades\\haarcascade_frontalface_alt.xml"))  //需要将xml文档放在自己指定的路径下
    {
       qDebug( "Load haarcascade_eye.xml failed!" );
    }
}

MainWindow::~MainWindow()
{
    if(matProThread->isRunning())
    {
        matProThread->stop();
        matProThread->wait();
    }
    delete matProThread;
    delete ui;
    //if exit with frame servive
    for(int i=0;i<frameBoxLife.size();i++)
    {
        delete frameBox[i];
        delete frameImg[i];
    }
}

/*
 * redefine the FRAMEINTERVAL to change the speed of refreash action
 */
void MainWindow::refresh()
{
    if(flagPlay)
    {
        //|0------1------2----------------45----|
        for(int i=0;i<frameBoxLife.size();i++)
        {
            QPoint pos = (ui->radioButton_start->pos() + (1.0-(frameBoxLife[i]/FRAMEBOX_TTL)) * (ui->radioButton_end->pos()-ui->radioButton_start->pos()));
            frameBox[i]->move(pos);
            frameImg[i]->move(pos+FRAMEINS_SHIFT);
            frameBoxLife[i] -= FRAMEINTERVAL;
            if(frameBoxLife[i] <= 0)
                delSnapshot(i);
        }
        //video play
        cv::Mat mat;
        matProThread->mutex.lock();
        if (0<matBuf.size())
        {
            mat = matBuf.dequeue();
            matProThread->mutex.unlock();
            if (!mat.empty())
            {
                //*************************************************************
                //add your Algorithm here
                mat = mat;
                vector<Rect> faceRect;
                cvtColor(mat, mat, CV_BGR2GRAY);//转为灰度图
                face_cascade.detectMultiScale(mat, faceRect, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
                for (size_t i = 0; i < faceRect.size(); i++)
                {
                    rectangle(mat, faceRect[i], Scalar(0, 0, 255));      //用矩形画出检测到的位置
                }
                Mat face = mat(faceRect[0]);
                Mat half_face=face(Range(0,face.rows*0.6),Range(0,face.cols));
                eye_cascade.detectMultiScale(half_face, faceRect, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
                for (size_t i = 0; i < faceRect.size(); i++)
                {
                    rectangle(half_face, faceRect[i], Scalar(0, 0, 255));      //用矩形画出检测到的位置
                }
                //*************************************************************
                imshow("o",mat);
                mainVideo = QtOcv::mat2Image(face);
                ui->label_video->setPixmap(QPixmap::fromImage(mainVideo).scaled(ui->spinBox_cols->value(),ui->spinBox_rows->value()));
            }
            else
                flagPlay=false;
        }
        else
            matProThread->mutex.unlock();

        ui->statusBar->showMessage("Buf of mats:"+QString::number(matBuf.size())+" fps:"+QString::number(1000.0/FRAMEINTERVAL));
    }
}

/*
 * consider the style in different situation here
 */
int MainWindow::addSnapshot(situation s)
{
    QString buttonText,buttonStyle;
    switch(s)
    {
    default:return 1;
    case BAD_IMG:buttonText="Bad Img";
        buttonStyle="color:black; background-color: yellow";
        break;
    case VERYBAD_IMG:buttonText="Very Bad";
        buttonStyle="color:white; background-color: blue";
        break;
    case OMG:buttonText=tr("有毛病");
        buttonStyle="color:white; background-color: red";
        break;
    }
    if (frameBoxLife.size()<FRAMEBOX_MAXNUM)
    {
        int ind=frameBoxLife.size();
        frameBoxLife << FRAMEBOX_TTL;
        frameBox << new QPushButton(buttonText,this);
        frameBox[ind]->move(ui->radioButton_start->pos());
        frameBox[ind]->setStyleSheet(buttonStyle);
        frameBox[ind]->show();
        frameImg << new QLabel(this);
        QImage clip = mainVideo.scaled(QSize(FRAMEIMG_W,FRAMEIMG_H));
        frameImg[ind]->resize(QSize(FRAMEIMG_W,FRAMEIMG_H));
        frameImg[ind]->setPixmap(QPixmap::fromImage(clip));
        frameImg[ind]->move(ui->radioButton_start->pos());
        connect(frameBox[ind],SIGNAL(pressed()),frameImg[ind],SLOT(show()));
        connect(frameBox[ind],SIGNAL(released()),frameImg[ind],SLOT(hide()));
    }
    return 0;
}

int MainWindow::delSnapshot(int ind)
{
    disconnect(frameBox[ind],SIGNAL(pressed()),frameImg[ind],SLOT(show()));
    disconnect(frameBox[ind],SIGNAL(released()),frameImg[ind],SLOT(hide()));
    frameBoxLife.remove(ind);
    delete frameBox[ind];
    frameBox.remove(ind);
    delete frameImg[ind];
    frameImg.remove(ind);
    return 0;
}

/*
 * judge the img here
 * Is an example in dif case
 */
void MainWindow::on_pushButton_snapshot_clicked()
{
    if(flagPlay)
        addSnapshot(situation(qrand()%3));
}

void MainWindow::on_pushButton_openVideo_clicked()
{
    //close first if open before
    flagPlay = false;
    if(video.isOpened())
        video.release();
    if(matProThread->isRunning())
    {
        matProThread->stop();
        matProThread->wait();
    }

    //open
    //|-------------|
    //|1 camerel    |
    //|2 file       |
    //|-------------|
    QStringList items;
    items << tr("Cameral") << tr("File") ;
    bool ok;
    QString item = QInputDialog::getItem(this, tr("Select a input:"),
                                         tr("input:"), items, 0, false, &ok);
    if (ok && !item.isEmpty())
    {
        if("File"==item){
            //ask user the loc of file
            QString videoFileName = QFileDialog::getOpenFileName(this, tr("Open a video file"));
            if (!videoFileName.isEmpty())
            {
                video = cv::VideoCapture(videoFileName.toStdString());
                if (video.isOpened())
                {
                    ui->statusBar->showMessage("Open file "+videoFileName + " Success");
                    matProThread->set(&video,&matBuf,maxBufSize);
                    matProThread->start();
                    flagPlay = true;
                }
                else
                    ui->statusBar->showMessage(videoFileName + " is not a good VIDEO file",1000);
            }
        }
        //Open the Cameral with index 0
        if("Cameral"==item)
        {
            video = cv::VideoCapture(0);
            if (video.isOpened())
            {
                ui->statusBar->showMessage("Open file cameral 0");
                matProThread->set(&video,&matBuf,maxBufSize);
                matProThread->start();
                flagPlay = true;
            }
            else
                ui->statusBar->showMessage("can't open cameral 0",1000);

        }
    }
}

void MainWindow::on_pushButton_pause_clicked()
{
    flagPlay = !flagPlay;
}
