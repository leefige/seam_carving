#ifndef REMOVE_H_
#define REMOVE_H_

#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define BRUSH_SIZE 10

struct mouseData {
	Vec3b color;
	Mat tmp;
	int rows, cols;
	int xmin, xmax, ymin, ymax;
	vector<pair<int, int> >& obj;

	mouseData(const Mat& src, vector<pair<int, int> >& mask):
		color(Vec3b(0, 0, 255)), obj(mask), xmin(RAND_MAX), ymin(RAND_MAX), xmax(-1), ymax(-1)
	{
		src.copyTo(tmp);
		rows = tmp.rows;
		cols = tmp.cols;
	}
};

class Remove
{
public:
	mouseData* data;
	Mat backup;
	
	Remove(const Mat& src, vector<pair<int, int> >& mask);
	virtual ~Remove();

	void removeObj();
};

void onMouse(int ev, int x, int y, int flag, void *data);

#endif