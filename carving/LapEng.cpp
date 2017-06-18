#include "LapEng.h"

LapEng::~LapEng(){}

void LapEng::getEnergy(const Mat & src, Mat & dst)
{
	Mat gradH(src.rows, src.cols, CV_32F, Scalar(0));
	Mat grey(src.rows, src.cols, CV_8U, Scalar(0));
	Mat tmp(src.rows, src.cols, src.type(), Scalar(0));

	GaussianBlur(src, tmp, Size(3, 3), 0, 0, BORDER_DEFAULT);	//denoise
	cvtColor(tmp, grey, CV_BGR2GRAY);	//convert to grey

	Laplacian(grey, gradH, gradH.depth(), 3);
	dst = abs(gradH);

	return;
}
