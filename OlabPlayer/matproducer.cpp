#include "matproducer.h"


MatProducer::MatProducer()
{

}

void MatProducer::set(VideoCapture *video, QQueue<Mat> *matBuf, int maxFrame)
{
    this->video=video;
    this->matBuf=matBuf;
    this->maxFrame=maxFrame;
    runFlag = true;
}

void MatProducer::run()
{
    //Mat mat; //if you struct mat here and use cameral funning thing will hapen :)
    while(runFlag)
    {
        if(matBuf->size()<maxFrame)
        {
            if(video->isOpened())
            {
                Mat mat;    //new mat every time
                mutex.lock();
                video->read(mat);
                if(!mat.empty())
                {
                    matBuf->enqueue(mat);
                }
                mutex.unlock();
            }
            else
                runFlag = false;
        }
        msleep(1);
    }
}

void MatProducer::stop()
{
    runFlag = false;
}
