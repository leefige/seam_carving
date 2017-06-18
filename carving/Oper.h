#ifndef OPER_H
#define OPER_H

#include <opencv2/opencv.hpp>
#include <vector>
#include "Energy.h"
using namespace cv;
using namespace std;

class Oper
{
public:
    int fac_x;
    int fac_y;
    Oper();
    virtual ~Oper();

    virtual void calcDif(const Mat & src, Mat & grad, Energy* en);
    virtual void show(Mat& tmp, vector<pair<int, int> > path) = 0;
    virtual void getTrace(Mat & grad, Mat& ener, Mat & trace) = 0;
    virtual vector<pair<int, int> > getPath(Mat & tracer) = 0;
    virtual void oneLine(const Mat & src, Mat & dst, vector<pair<int, int> > path) = 0;
};

#endif // OPER_H
