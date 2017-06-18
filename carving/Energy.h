#ifndef ENERGY_H_
#define ENERGY_H_

#include <opencv2/opencv.hpp>
using namespace cv;

class Energy
{
public:

	Energy() {};

	virtual ~Energy(){}

	virtual void getEnergy(const Mat& src, Mat & dst) = 0;
};

#endif

