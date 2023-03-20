#include "image.h"
using namespace std;
using namespace cv;

Image::Image(){}

void Image::setImage(Mat& img)
{
    this->originalImg = img.clone();
    this->outputImage = img.clone();

}

void Image::reset(){
    this->outputImage = this->originalImg.clone();

}

Mat& Image::getOutputImg(){
    return this->outputImage;
}

Mat& Image::getOriginalImage(){
    return this->originalImg;
}



