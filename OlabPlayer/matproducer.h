#ifndef MATPRODUCER_H
#define MATPRODUCER_H
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <QThread>
#include <QQueue>

using namespace cv;

class MatProducer : public QThread
{
public:
    MatProducer(VideoCapture *video, QQueue<Mat> *matBuf, int maxFrame);
    void stop(void);
    void run();

private:
    VideoCapture *video;
    QQueue<Mat> *matBuf;
    int maxFrame;
    volatile bool runFlag;
};

#endif // MATPRODUCER_H
