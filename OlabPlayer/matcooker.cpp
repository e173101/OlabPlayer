#include "matcooker.h"

MatCooker::MatCooker()
{

}

Mat MatCooker::cook(Mat mat)
{
    if(!mat.empty())
    {
        Mat imageRGB[3];
        split(mat, imageRGB);
        return imageRGB[0];
    }
    return mat;
}
