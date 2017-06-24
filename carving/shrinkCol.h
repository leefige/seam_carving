#ifndef SHRINKCOL_H
#define SHRINKCOL_H

#include "Oper.h"


class shrinkCol : public Oper
{
public:
    shrinkCol();
    virtual ~shrinkCol();

    void show(Mat& tmp, vector<pair<int, int> > path);
    void getTrace(Mat & grad, Mat& ener, Mat & trace);
    vector<pair<int, int> > getPath(const Mat& ener, Mat & tracer);
    void oneLine(const Mat & src, Mat & dst, vector<pair<int, int> > path);
};

#endif // SHRINKCOL_H
