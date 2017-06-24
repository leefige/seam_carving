#include "func.h"

Energy* energyType(string type)
{
	if (type == "sobel") {		//sobel op
		return new SobelEng;
	}
	else if (type == "lap") {	//laplace op
		return new LapEng;
	}
	else if (type == "rob") {	//roberts op
		return new RobEng;
	}
	return NULL;
}

Mat removeObj(const Mat& src, Energy* en, Oper* op, Remove* rm)
{
	imshow("Source image", src);
	cvWaitKey(64);
	//printf("shown\n");
	Mat tmp;
	src.copyTo(tmp);
	Mat& bk = rm->backup;
	Mat out;
	int ed = rm->data->ymax;
	int st = rm->data->ymin;
	//printf("st, ed %d %d\n", st, ed);
	bool finish = 0;

	//while(!finish)
	for(int i = st; i < ed + 3; i++)
	{
		//printf("line = %d", i);
		finish = 1;
		Mat grad(tmp.rows, tmp.cols, CV_32F, Scalar(0));
		op->calcDif(tmp, grad, en);
		for (int i = 0; i < bk.rows; i++)
		{
			for (int j = 0; j < bk.cols; j++)
			{
				if (bk.at<uchar>(i, j) == 1)
				{
					finish = 0;
					//printf("i, j %d %d\n", i, j);
					grad.at<float>(i, j) = -0xffff;
				}
			}
		}
		//printf("diffed\n");
		Mat trace(tmp.rows, tmp.cols, CV_32S, Scalar(0));
		Mat ener(tmp.rows, tmp.cols, CV_32F, Scalar(0));
		op->getTrace(grad, ener, trace);
		vector< pair<int, int> > path(op->getPath(ener, trace));

		/*-------------show seam-------------------*/
		//printf("show seam\n");
		op->show(tmp, path);

		Mat dst(tmp.rows + op->fac_x, tmp.cols + op->fac_y, tmp.type());    //col - 1
		Mat bk_dst(tmp.rows + op->fac_x, tmp.cols + op->fac_y, tmp.type());    //col - 1
		op->oneLine(tmp, dst, path);
		op->oneLine(bk, bk_dst, path);
		dst.copyTo(out);
		tmp = out;
		bk_dst.copyTo(bk);
	}
	return out;
}

Mat resizeImg(const Mat & src, int new_size, Energy* en, Oper * op)
{
	imshow("Source image", src);
	cvWaitKey(64);
	//printf("shown\n");
	Mat tmp;
	src.copyTo(tmp);
	Mat out;
	int ed = tmp.cols > new_size ? tmp.cols : new_size;
	int st = tmp.cols < new_size ? tmp.cols : new_size;
	for (int i = ed; i > st; i--)
	{
		//printf("line = %d", i);
		Mat grad(tmp.rows, tmp.cols, CV_32F, Scalar(0));
		op->calcDif(tmp, grad, en);
        //printf("diffed\n");
		Mat trace(tmp.rows, tmp.cols, CV_32S, Scalar(0));
		Mat ener(tmp.rows, tmp.cols, CV_32F, Scalar(0));
		op->getTrace(grad, ener, trace);
		vector< pair<int, int> > path(op->getPath(ener, trace));

        /*-------------show seam-------------------*/
        //printf("show seam\n");
        op->show(tmp, path);

		Mat dst(tmp.rows + op->fac_x, tmp.cols + op->fac_y, tmp.type());    //col - 1
		op->oneLine(tmp, dst, path);
		dst.copyTo(out);
		tmp = out;
	}
	return out;
}
