#include "matproducer.h"


MatProducer::MatProducer(VideoCapture *video, QQueue<Mat> *matBuf, int maxFrame)
{
    this->video=video;
    this->matBuf=matBuf;
    this->maxFrame=maxFrame;
    runFlag = true;
}

void MatProducer::run()
{
    Mat mat;
    while(runFlag)
    {
        if(matBuf->size()<maxFrame)
        {
            video->read(mat);
            if(!mat.empty())
                matBuf->enqueue(mat);
            else
                runFlag = false;
        }
    }
}

void MatProducer::stop()
{
    runFlag = false;
}
