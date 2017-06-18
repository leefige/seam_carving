#include "enlargeCol.h"
#include <opencv2/opencv.hpp>

enlargeCol::enlargeCol()
{
    //ctor
    fac_x = 0;
    fac_y = 1;
}

enlargeCol::~enlargeCol()
{
    //dtor
}

void enlargeCol::getTrace(Mat & grad, Mat& ener, Mat & trace)
{
	int crow = 1;	//currgradt row, from second row
	int rows = grad.rows;
	int cols = grad.cols;
	grad.copyTo(ener);

	for (crow; crow < rows; crow++) {
		/*--------------------first col--------------------------*/

		//for place, -1:up left, 0:up mid, 1:up right
		//printf("crow= %d\n", crow);
		//printf("a: %f\n", ener.at<unsigned int>(crow - 1, 0));

		if (ener.at<float>(crow - 1, 0) > ener.at<float>(crow - 1, 1)) {
			ener.at<float>(crow, 0) += ener.at<float>(crow - 1, 1);	//right
			trace.at<int>(crow, 0) = 1;
			//trace.push_back(pair<int, int>(crow - 1, 1));
		}
		else {
			ener.at<float>(crow, 0) += ener.at<float>(crow - 1, 0);	//mid
			trace.at<int>(crow, 0) = 0;
			//trace.push_back(pair<int, int>(crow - 1, 0));
		}
		//printf("first line\n");
		/*--------------------others-----------------------------*/

		int j = 1;	//currgradt col
		for (j; j < cols - 1; j++)
		{
			float tmp[3];
			for (int k = 0; k < 3; k++) {
				tmp[k] = ener.at<float>(crow - 1, j - 1 + k);
			}

			int min = 0;
			if (tmp[1] < tmp[0])
				min = 1;
			if (tmp[2] < tmp[min])
				min = 2;

			ener.at<float>(crow, j) += ener.at<float>(crow - 1, j - 1 + min);
			trace.at<int>(crow, j) = min - 1;
			//trace.push_back(pair<int, int>(crow - 1, j - 1 + min));
		}

		/*-----------------last col, j == col-2----------------------*/

		if (ener.at<float>(crow - 1, j) > ener.at<float>(crow - 1, cols - 1)) {
			ener.at<float>(crow, cols - 1) += ener.at<float>(crow - 1, cols - 1);	//mid
			trace.at<int>(crow, j) = 0;
			//trace.push_back(pair<int, int>(crow - 1, cols - 1));
		}
		else {
			ener.at<float>(crow, cols - 1) += ener.at<float>(crow - 1, j);	//left
			trace.at<int>(crow, j) = -1;
			//trace.push_back(pair<int, int>(crow - 1, j));
		}
	}//gradd for
}

vector<pair<int, int> > enlargeCol::getPath(Mat & tracer)
{
	vector<pair<int, int> > buf;

    int crow = tracer.rows - 1;	//current row is last row
	int pos = tracer.cols - 1;				//start point
	float min = RAND_MAX;
	int c = rand() % 2;
	for (int i = 0; i < tracer.cols - 1; i++)
	{
		if (((c == 0) && (tracer.at<float>(crow, i) <= min)) ||
            ((c == 1) && (tracer.at<float>(crow, i) < min)))
		{
			pos = i;
			min = tracer.at<float>(crow, i);
		}
	}
	buf.push_back(pair<int, int>(crow, pos));
	crow--;

	int place = pos;			//place of next point
	for (crow; crow >= 0; crow--)
	{
		int re = tracer.at<int>(crow, place);	// relative place
		place += re;
		buf.push_back(pair<int, int>(crow, place));
	}

	return vector<pair<int, int> >(buf);
}

void enlargeCol::oneLine(const Mat & src, Mat & dst, vector<pair<int, int> > path)
{
	/*-------------cut one col------------------*/
	int rows = src.rows;
	Mat kernel = (Mat_<float>(3,3) << 1.5,  2,  1,
                                      3,    4,  2,
                                      1.5,  2,  1);
    for (int i = 0; i < rows; i++)	//each row
	{
		int fst = path[i].first;
		int scd = path[i].second;
		int j = 0;
		for (j; j < scd; j++)	//first part
		{
			dst.at<Vec3b>(fst, j)[0] = src.at<Vec3b>(fst, j)[0];
			dst.at<Vec3b>(fst, j)[1] = src.at<Vec3b>(fst, j)[1];
			dst.at<Vec3b>(fst, j)[2] = src.at<Vec3b>(fst, j)[2];
		}

		//specific one line
		if(fst == 0 || fst == rows - 1)
		{
		    dst.at<Vec3b>(fst, j)[0] = int((1.5*src.at<Vec3b>(fst, j - 1)[0] + 2*src.at<Vec3b>(fst, j)[0] + 1*src.at<Vec3b>(fst, j + 1)[0]) / 4.5);
            dst.at<Vec3b>(fst, j)[1] = int((1.5*src.at<Vec3b>(fst, j - 1)[1] + 2*src.at<Vec3b>(fst, j)[1] + 1*src.at<Vec3b>(fst, j + 1)[1]) / 4.5);
            dst.at<Vec3b>(fst, j)[2] = int((1.5*src.at<Vec3b>(fst, j - 1)[2] + 2*src.at<Vec3b>(fst, j)[2] + 1*src.at<Vec3b>(fst, j + 1)[2]) / 4.5);

		}
        else
        {
            dst.at<Vec3b>(fst, j)[0] = int((1.5*src.at<Vec3b>(fst - 1, j - 1)[0] + 2*src.at<Vec3b>(fst - 1, j)[0] + 1*src.at<Vec3b>(fst - 1, j + 1)[0] +
                                            3.0*src.at<Vec3b>(fst, j - 1)[0]     + 4*src.at<Vec3b>(fst, j)[0]     + 2*src.at<Vec3b>(fst, j + 1)[0] +
                                            1.5*src.at<Vec3b>(fst + 1, j - 1)[0] + 2*src.at<Vec3b>(fst + 1, j)[0] + 1*src.at<Vec3b>(fst + 1, j + 1)[0]) / 18);

            dst.at<Vec3b>(fst, j)[1] = int((1.5*src.at<Vec3b>(fst - 1, j - 1)[1] + 2*src.at<Vec3b>(fst - 1, j)[1] + 1*src.at<Vec3b>(fst - 1, j + 1)[1] +
                                            3.0*src.at<Vec3b>(fst, j - 1)[1]     + 4*src.at<Vec3b>(fst, j)[1]     + 2*src.at<Vec3b>(fst, j + 1)[1] +
                                            1.5*src.at<Vec3b>(fst + 1, j - 1)[1] + 2*src.at<Vec3b>(fst + 1, j)[1] + 1*src.at<Vec3b>(fst + 1, j + 1)[1]) / 18);

            dst.at<Vec3b>(fst, j)[2] = int((1.5*src.at<Vec3b>(fst - 1, j - 1)[2] + 2*src.at<Vec3b>(fst - 1, j)[2] + 1*src.at<Vec3b>(fst - 1, j + 1)[2] +
                                            3.0*src.at<Vec3b>(fst, j - 1)[2]     + 4*src.at<Vec3b>(fst, j)[2]     + 2*src.at<Vec3b>(fst, j + 1)[2] +
                                            1.5*src.at<Vec3b>(fst + 1, j - 1)[2] + 2*src.at<Vec3b>(fst + 1, j)[2] + 1*src.at<Vec3b>(fst + 1, j + 1)[2]) / 18);
        }

		j++;

		for (j; j < dst.cols; j++)  //second part
		{
			dst.at<Vec3b>(fst, j)[0] = src.at<Vec3b>(fst, j - 1)[0];
			dst.at<Vec3b>(fst, j)[1] = src.at<Vec3b>(fst, j - 1)[1];
			dst.at<Vec3b>(fst, j)[2] = src.at<Vec3b>(fst, j - 1)[2];
		}
	}
}

void enlargeCol::show(Mat& tmp, vector<pair<int, int> > path)
{
    Mat tp;
    tmp.copyTo(tp);
    int rows = tp.rows;
    for (int i = 0; i < rows; i++)
    {
        int fst = path[i].first;
        int scd = path[i].second;
        tp.at<Vec3b>(fst, scd)[0] = 0;     //blue
        tp.at<Vec3b>(fst, scd)[1] = 255;   //green
        tp.at<Vec3b>(fst, scd)[2] = 0;     //red
    }
    imshow("Seam", tp);
    cvWaitKey(100);
}

