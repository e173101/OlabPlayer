#include "matcooker.h"

MatCooker::MatCooker():
    rng(0xFFFFFFFF)
{

}

/****
 * @brief: the place you Test your algorithm here
 * @param mat: what you want to do for me？
 * @retrun mat: algorithm result
 ****/

Scalar randomColor( RNG& rng )
{
    int icolor = (unsigned) rng;
    return Scalar( icolor&255, (icolor>>8)&255, (icolor>>16)&255 );
}

int Drawing_Random_Lines(Mat image, RNG& rng)
{
    Point pt1,pt2;
    for(int i=0; i<10; i++)
    {
     pt1.x = rng.uniform( 0, image.cols );
     pt1.y = rng.uniform( 0, image.rows);
     pt2.x = rng.uniform( 0, image.cols );
     pt2.y = rng.uniform( 0, image.rows );
     line( image, pt1, pt2, randomColor(rng), rng.uniform(1, 10), 8 );
    }
    return 0;
}

void MatCooker::cook(Mat mat)
{
    if(mat.empty()) return;

    Mat imageRGB[3];
    split(mat, imageRGB);
    rotate(imageRGB[0],imageRGB[0],1);
    flip(imageRGB[1],imageRGB[1],1);
    Size sz = mat.size();
    Mat R = Mat(sz,CV_8UC3);
    randu(R,Scalar::all(0),Scalar::all(100));
    merge(imageRGB,3,mat);
    mat = mat + R;
    Drawing_Random_Lines(mat,rng);
}
