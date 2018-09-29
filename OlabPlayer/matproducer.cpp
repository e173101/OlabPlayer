/* 视频生产者
 * 输入视频的路径，尽力把所有Mat传给matcooker
 *
 *
 */
#include "matproducer.h"

MatProducer::MatProducer()
{

}

void MatProducer::setWaitUI(bool wait)
{
    this->waitUIFlag = wait;
}

void MatProducer::run()
{
    //if you struct mat here and use cameral funning thing will hapen :)
    runFlag = true;
    waitUIFlag = true;
    UItakenFlag = false;
    frameNum = 0;

    qDebug("cook mat begin");
    while(runFlag)
    {
        if(waitUIFlag)
        {
            if(!UItakenFlag)    //haven taked
            {
                mutex.lock();
                video.read(mat);
                mutex.unlock();
                UItakenFlag = true;
                matcooker.cook(mat);
                //cook ready UI can use the mat
                frameNum++;
            }
            msleep(1);
        }
        else
        {
            video.read(mat);
            matcooker.cook(mat);
            frameNum++;
        }
    }
}

//tell the thread I'd read the mat
void MatProducer::getOneMat(void)
{
    if(UItakenFlag)
    {
        UItakenFlag = false;
    }
}

void MatProducer::stop()
{
    runFlag = false;
}
