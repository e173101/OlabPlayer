/* 视频生产者
 * 输入视频的路径，尽力把所有Mat传给matcooker
 * 统一大小以适应不同摄像头
 *
 */
#include "matproducer.h"

MatProducer::MatProducer():
    matSize(Size(320,240))
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
                resize(mat,mat,matSize);
                //matcooker.cook(mat);
                UItakenFlag = true;
                mutex.unlock();
                //cook ready UI can use the mat
                frameNum++;
            }
            msleep(1);
        }
        else
        {
            video.read(mat);
            resize(mat,mat,matSize);
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
