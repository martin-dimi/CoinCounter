#include <stdio.h>
#include "includes.h"

#define _USE_MATH_DEFINES


#include <math.h> 

using namespace cv;

Mat calculateGradientMagnitude(Mat &dx, Mat &dy);
Mat calculateGradientDirection(Mat &dx, Mat &dy);

Mat applyKernel(int kernel[3][3], Mat &originalImage);
uchar normaliseUcharGray(float max, float min, int x);
Mat imageWrite(Mat &image, std::string imagename);