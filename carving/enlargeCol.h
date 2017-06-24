#ifndef ENLARGECOL_H
#define ENLARGECOL_H

#include "Oper.h"


class enlargeCol : public Oper
{
public:
    enlargeCol();
    virtual ~enlargeCol();

    void show(Mat& tmp, vector<pair<int, int> > path);
    void getTrace(Mat & grad, Mat& ener, Mat & trace);
    vector<pair<int, int> > getPath(const Mat& ener, Mat & tracer);
    void oneLine(const Mat & src, Mat & dst, vector<pair<int, int> > path);
};

#endif // ENLARGECOL_H
