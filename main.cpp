#include "Sobel.hpp"

using namespace cv;

int dx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

int dy[3][3] = {
    {-1, -2, -1},
    {0, 0, 0},
    {1, 2, 1}
};

int main( int argc, char** argv )
{

    // LOADING THE IMAGE
	char* imageName = argv[1];

	Mat image;
	image = imread( imageName, 1 );

	if( argc != 2 || !image.data )
	{
		printf( " No image data \n " );
		return -1;
	}

	// CONVERT COLOUR, BLUR AND SAVE
	Mat gray_image;
	cvtColor( image, gray_image, CV_BGR2GRAY );

    Mat dxImage = applyKernel(dx, gray_image);
    Mat dyImage = applyKernel(dy, gray_image);

    Mat magnitude = calculateGradientMagnitude(dxImage, dyImage);
    Mat direction = calculateGradientDirection(dxImage, dyImage);

    imageWrite(dxImage, "xGradient.jpg");
    imageWrite(dyImage, "yGradient.jpg");
    // magnitude = imageWrite(magnitude, "magnitudeGradient.jpg");
    imageWrite(direction, "directionGradient.jpg");


    for(int x = 0; x < magnitude.rows; x++) {	
		for(int y = 0; y < magnitude.cols; y++) {
            if(magnitude.at<float>(x, y) > 300)
                magnitude.at<float>(x, y) = 255;
            else
                magnitude.at<float>(x, y) = 0;
        }
    }

    imageWrite(magnitude, "magnitudeGradientThreshold.jpg");
    return 0;
}