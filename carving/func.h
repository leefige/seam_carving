#ifndef FUNC_H_
#define FUNC_H_

#include <opencv2/opencv.hpp>
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <vector>

#include "Remove.h"
#include "Energy.h"
#include "LapEng.h"
#include "SobelEng.h"
#include "RobEng.h"

#include "Oper.h"
#include "shrinkCol.h"
#include "enlargeCol.h"

using namespace cv;
using namespace std;

Energy * energyType(string type);

Mat removeObj(const Mat& src, Energy* en, Oper* op, Remove* rm);
Mat resizeImg(const Mat& src, int new_size, Energy* en, Oper* op);

#endif
