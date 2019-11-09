#include "Sobel.hpp"

using namespace cv;

void calculateHough(Mat& magnitude, Mat& direction, int ***hough, int radiusMax);
void visualiseHough(int ***hough, int rows, int cols, int radiusMax);

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

const auto PI = std::acos(-1);

int ***malloc3dArray(int dim1, int dim2, int dim3)
{
    int i, j, k;
    int ***array = (int ***) malloc(dim1 * sizeof(int **));

    for (i = 0; i < dim1; i++) {
        array[i] = (int **) malloc(dim2 * sizeof(int *));
        for (j = 0; j < dim2; j++) {
            array[i][j] = (int *) malloc(dim3 * sizeof(int));
            memset(array[i][j], 0, dim3*sizeof(int));
        }   

    }
    return array;
}

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
    imageWrite(magnitude, "magnitudeGradient.jpg");
    imageWrite(direction, "directionGradient.jpg");
    
    int rows = magnitude.rows;
    int cols = magnitude.cols;
    int radiusMax = 150;

    int*** hough = malloc3dArray(rows, cols, radiusMax);

    magnitude = imageWrite(magnitude, "magnitudeGradient.jpg");
    // normalize(magnitude, magnitude, 0, 255, cv::NORM_MINMAX);

    for(int x = 0; x < rows; x++) {	
		for(int y = 0; y < cols; y++) {
            if(magnitude.at<float>(x, y) > 60)
                magnitude.at<float>(x, y) = 255;
            else
                magnitude.at<float>(x, y) = 0;
        }
    }

    calculateHough(magnitude, direction, hough, radiusMax);

    visualiseHough(hough, rows, cols, radiusMax);

    imageWrite(magnitude, "magnitudeGradientThreshold.jpg");

    return 0;
}

void visualiseHough(int ***hough, int rows, int cols, int radiusMax) {

    Mat houghImage = Mat(rows, cols, CV_32FC1, Scalar(0));

    printf("Cols: %d, Rows: %d\n", cols, rows);

    for(int x = 1; x < rows-1; x++) {	
        for(int y = 1; y < cols-1; y++) {
            float result = 0;
            int max = 0;
            for(int r = 0; r < radiusMax; r++) {
                result += hough[x][y][r];
                if(hough[x][y][r] > max) max = hough[x][y][r];
            }
        
            // Colapse the 3d Hough transform into 2d
            houghImage.at<float>(x,y) = result;
            float temp = houghImage.at<float>(x,y);
            // printf("Result is: %f and hough is: %f, max is %d\n", result, temp, max);
        }
    }

    imageWrite(houghImage, "houghSpace.jpg");
}

void calculateHough(Mat& magnitude, Mat& direction, int ***hough, int radiusMax) {
    int rows = magnitude.rows;
    int cols = magnitude.cols;

    for(int x = 1; x < magnitude.rows - 1; x++) {	
        for(int y = 1; y < magnitude.cols - 1; y++) {

            if(magnitude.at<float>(x,y) <= 10)
                continue;

            float dir = direction.at<float>(x,y);

            for(int radius = 10; radius < radiusMax - 10; radius++) {

                int x0p = x + radius * cos(dir);
                int x0m = x - radius * cos(dir);

                int y0p = y + radius * sin(dir);
                int y0m = y - radius * sin(dir);

                // X+ Y+
                if(x0p >= 0 && x0p < rows && y0p >= 0 && y0p < cols) {
                    hough[x0p][y0p][radius - 10] += 1;
                }

                // X+ Y-
                if(x0p >= 0 && x0p < rows && y0m >= 0 && y0m < cols) {
                    hough[x0p][y0m][radius - 10] += 1;
                }

                // X- Y+
                if(x0m >= 0 && x0m < rows && y0p >= 0 && y0p < cols) {
                    hough[x0m][y0p][radius - 10] += 1;
                }

                // X- Y-
                if(x0m >= 0 && x0m < rows && y0m >= 0 && y0m < cols) {
                    hough[x0m][y0m][radius - 10] += 1;
                }

            }
            
        }
            // printf("%d ", t);
    }

}