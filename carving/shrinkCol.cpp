#include "shrinkCol.h"

shrinkCol::shrinkCol()
{
    //ctor
    fac_x = 0;
    fac_y = -1;
}

shrinkCol::~shrinkCol()
{
    //dtor
}

void shrinkCol::getTrace(Mat & grad, Mat& ener, Mat & trace)
{
	//printf("getTrace called\n");
	int crow = 1;	//currgradt row, from second row
	int rows = grad.rows;
	int cols = grad.cols;
	grad.copyTo(ener);

	for (crow; crow < rows; crow++) {
		/*--------------------first col--------------------------*/

		//for place, -1:up left, 0:up mid, 1:up right
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
		for (j; j < cols - 1; j++)	{
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
		}

		/*-----------------last col, j == col-2----------------------*/

		if (ener.at<float>(crow - 1, j) > ener.at<float>(crow - 1, cols - 1)) {
			ener.at<float>(crow, cols - 1) += ener.at<float>(crow - 1, cols - 1);	//mid
			trace.at<int>(crow, j) = 0;
		}
		else {
			ener.at<float>(crow, cols - 1) += ener.at<float>(crow - 1, j);	//left
			trace.at<int>(crow, j) = -1;
		}
	}//gradd for
}

vector<pair<int, int> > shrinkCol::getPath(Mat & tracer)
{
    //printf("getPath called\n");
	vector<pair<int, int> > buf;

    int crow = tracer.rows - 1;	//current row is last row
	int pos = tracer.cols - 1;	//start point
	float min = RAND_MAX;
	int c = rand() % 2;
	for (int i = 0; i < tracer.cols - 1 ; i++)
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

void shrinkCol::oneLine(const Mat & src, Mat & dst, vector<pair<int, int> > path)
{
    //printf("cutCol called\n");
	/*-------------cut one col------------------*/
	int rows = src.rows;
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

		for (j; j < dst.cols; j++)  //second part
		{
			dst.at<Vec3b>(fst, j)[0] = src.at<Vec3b>(fst, j + 1)[0];
			dst.at<Vec3b>(fst, j)[1] = src.at<Vec3b>(fst, j + 1)[1];
			dst.at<Vec3b>(fst, j)[2] = src.at<Vec3b>(fst, j + 1)[2];
		}
	}
}

void shrinkCol::show(Mat& tmp, vector<pair<int, int> > path)
{
    int rows = tmp.rows;
    for (int i = 0; i < rows; i++)
    {
        int fst = path[i].first;
        int scd = path[i].second;
        tmp.at<Vec3b>(fst, scd)[0] = 0;     //blue
        tmp.at<Vec3b>(fst, scd)[1] = 0;     //green
        tmp.at<Vec3b>(fst, scd)[2] = 255;   //red
    }
    imshow("Seam", tmp);
    cvWaitKey(32);
}
