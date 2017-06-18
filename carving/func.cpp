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
	Mat out;
	int ed = rm->data->xmax;
	int st = rm->data->xmin;

	while(!rm->data->obj.empty())
	{
		//printf("line = %d", i);
		Mat grad(tmp.rows, tmp.cols, CV_32F, Scalar(0));
		op->calcDif(tmp, grad, en);
		for (int i = 0; i < rm->data->obj.size(); i++)
		{
			grad.at<float>(rm->data->obj[i].first, rm->data->obj[i].second) = 0;
		}

		//printf("diffed\n");
		Mat trace(tmp.rows, tmp.cols, CV_32F, Scalar(0));
		Mat ener(tmp.rows, tmp.cols, CV_32F, Scalar(0));
		op->getTrace(grad, ener, trace);
		vector< pair<int, int> > path(op->getPath(trace));

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
		Mat trace(tmp.rows, tmp.cols, CV_32F, Scalar(0));
		Mat ener(tmp.rows, tmp.cols, CV_32F, Scalar(0));
		op->getTrace(grad, ener, trace);
		vector< pair<int, int> > path(op->getPath(trace));

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
