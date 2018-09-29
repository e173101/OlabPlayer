#include "matcooker.h"

MatCooker::MatCooker()
{

}

/****
 * @brief: the place you Test your algorithm here
 * @param mat: what you want to do for me？
 * @retrun mat: algorithm result
 ****/

void MatCooker::cook(Mat mat)
{
    if(mat.empty()) return;

    Mat imageRGB[3];
    split(mat, imageRGB);
    rotate(imageRGB[0],imageRGB[0],1);
    flip(imageRGB[1],imageRGB[1],1);
    merge(imageRGB,3,mat);
}
