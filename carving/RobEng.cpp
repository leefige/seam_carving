#include "RobEng.h"

RobEng::~RobEng(){}

void RobEng::getEnergy(const Mat & src, Mat & dst)
{
	Mat grey(src.rows, src.cols, CV_8U, Scalar(0));
	Mat tmp(src.rows, src.cols, src.type(), Scalar(0));

	GaussianBlur(src, tmp, Size(3, 3), 0, 0, BORDER_DEFAULT);	//denoise
	cvtColor(tmp, grey, CV_BGR2GRAY);	//convert to grey

    int rows = src.rows;
    int cols = src.cols;
    for (int i = 0; i < rows-1; i++)
    {
        for (int j = 0; j < cols-1; j++)
        {
            int t1 = (grey.at<uchar>(i, j) - grey.at<uchar>(i+1, j+1)) * (grey.at<uchar>(i, j) - grey.at<uchar>(i+1, j+1));
            int t2 = (grey.at<uchar>(i+1, j) - grey.at<uchar>(i, j+1)) * (grey.at<uchar>(i+1, j) - grey.at<uchar>(i, j+1));
            dst.at<float>(i, j) = float(sqrt(t1 + t2));
        }
    }
	return;
}
