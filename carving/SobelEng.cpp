#include "SobelEng.h"

SobelEng::~SobelEng(){}

void SobelEng::getEnergy(const Mat & src, Mat & dst)
{
	Mat gradH(src.rows, src.cols, CV_32F, Scalar(0));
	Mat gradV(src.rows, src.cols, CV_32F, Scalar(0));
	Mat grey(src.rows, src.cols, CV_8U, Scalar(0));
	Mat tmp(src.rows, src.cols, src.type(), Scalar(0));

	GaussianBlur(src, tmp, Size(3, 3), 0, 0, BORDER_DEFAULT);	//denoise
	cvtColor(tmp, grey, CV_BGR2GRAY);	//convert to grey

	Sobel(grey, gradH, gradH.depth(), 1, 0);
	Sobel(grey, gradV, gradV.depth(), 0, 1);
	add(abs(gradH), abs(gradV), dst);
	//addWeighted(abs(gradH), 0.5, abs(gradV), 0.5, 0, dst);
	return;
}
