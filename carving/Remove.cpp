#include "Remove.h"



Remove::Remove(const Mat& src, vector<pair<int, int> >& mask)
{
	data = new mouseData(src, mask);
}


Remove::~Remove()
{
	vector<pair<int, int> > tp;
	data->obj.swap(tp);
	if (data)
		delete data;
}

void Remove::removeObj()
{
	namedWindow("Draw mask", CV_WINDOW_AUTOSIZE);
	setMouseCallback("Draw mask", onMouse, (void*)data);
	while (1)
	{
		imshow("Draw mask", data->tmp);
		if (waitKey(100) == 27)
		{
			break;
		}
	}
}

void onMouse(int ev, int x, int y, int flag, void *data)
{
	mouseData * mdata = (mouseData *)data;
	vector<pair<int, int> >::iterator it;
	if ((ev == CV_EVENT_MOUSEMOVE || ev == CV_EVENT_LBUTTONDOWN) &&
		(flag & CV_EVENT_FLAG_LBUTTON))
	{
		int rad = BRUSH_SIZE;	//brush size
		for (int i = max(0, y - rad); i < min(mdata->rows, y + rad); i++)
		{
			int length = sqrt(pow(rad, 2) - pow(i - y, 2));
			for (int j = max(0, x - length); j < min(mdata->cols, x + length); j++)
			{
				it = find(mdata->obj.begin(), mdata->obj.end(), pair<int, int>(i, j));
				if (it == mdata->obj.end())
				{
					mdata->tmp.at<Vec3b>(i, j) = mdata->tmp.at<Vec3b>(i, j) * 0.7 + mdata->color * 0.3;
					mdata->obj.push_back(pair<int, int>(i, j));
					if (i < mdata->xmin)
						mdata->xmin = i;
					else if (i > mdata->xmax)
						mdata->xmax = i;
					if (j < mdata->ymin)
						mdata->ymin = j;
					else if (j > mdata->ymax)
						mdata->ymax = j;
				}
			}// end for
		}// end for
	}
}
