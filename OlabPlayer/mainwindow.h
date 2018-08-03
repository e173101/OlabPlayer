#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QPushButton>
#include <QLabel>
#include <QInputDialog>
#include <QFileDialog>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc.hpp"

#include "matproducer.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void refresh(void);
    void refresh_1s(void);

    void on_pushButton_snapshot_clicked();

    void on_pushButton_openVideo_clicked();

    void on_pushButton_pause_clicked();

private:

    //All kinds of situations
    enum situation{
        BAD_IMG,
        VERYBAD_IMG,
        OMG
    };

    Ui::MainWindow *ui;
    QTimer timer,timer_1s;
    QVector<int> frameBoxLife;
    QVector<QPushButton*> frameBox;
    QVector<QLabel*> frameImg;

    int addSnapshot(situation s);
    int delSnapshot(int ind);
    cv::VideoCapture video;
    QImage mainVideo;
    bool flagPlay;

    MatProducer *matProThread;
    QQueue<Mat> matBuf;
    int maxBufSize;

    /**********************************
     *
     *
     */
    CascadeClassifier eye_cascade;  //载入分类器
    CascadeClassifier face_cascade;    //载入分类器

};

#endif // MAINWINDOW_H
