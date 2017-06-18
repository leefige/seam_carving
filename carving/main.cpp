#include "func.h"
#include <direct.h>
#include <string.h>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	/*------------------process input-------------------------------*/
	if (argc < 2 || argc > 3) {
		printf("\nUsage: %s input_file\n\n", argv[0]);
		return 1;
	}

	string src = argv[1];		//source file
	char _dir[_MAX_PATH];
	string dir(_getcwd(_dir, _MAX_PATH));			//current dir
	const string src_dir = dir + string("/img/") + src;	//src dir
	const string out_dir = dir + string("/output/");		//output dir

	Mat img = imread(src_dir);	//image
	if (!img.data)
	{
		printf("\ninput file is empty or do not exist\n\n");
		return 2;
	}
	printf("rows: %d, cols: %d\n\n", img.rows, img.cols);

	Energy * gr = NULL; 
	Energy * gr2 = NULL;
	Remove* rm = NULL;
	Oper* op_col = NULL;
	Oper* op_row = NULL;
	int s_x, s_y;	//size_x, size_y
	bool isRemove = 0;
	string type;
	vector<pair<int, int> > mask;
	Mat buf;
	Mat output;

	//---------remove------------
	if (argc == 3)
	{
		isRemove = 1;
		printf("Please draw a mask to remove\n\n");
		gr = energyType("lap");
		gr2 = energyType("lap");
		type = "remove";
		rm = new Remove(img, mask);
		rm->removeObj();
		s_x = img.rows - (rm->data->xmax - rm->data->xmin);
		s_y = img.cols - (rm->data->ymax - rm->data->ymin);
		//------------------------------------------------

		op_col = new shrinkCol;		//use shrinkCol to remove object
		Mat mid(resizeImg(img, s_y, gr, op_col));
		mid.copyTo(buf);
	}

	//---------or: resize-----------
	else if (argc == 2)
	{
		char _type[10];
		printf("Please input: size_x size_y operator_type_from_{sobel, lap, rob}\n\n");
		cin >> s_x >> s_y >> _type;
		type = _type;	//operator type
		//cout << s_x << s_y << _type << endl;
		gr = energyType(type);
		gr2 = energyType(type);

		/*------------------col-------------------------*/
		
		if (s_y < img.cols)
			op_col = new shrinkCol;
		else if (s_y > img.cols)
			op_col = new enlargeCol;
		if (op_col)
		{
			Mat mid(resizeImg(img, s_y, gr, op_col));
			mid.copyTo(buf);
		}

		else
		{
			img.copyTo(buf);
		}

		/*-----------------row---------------------------*/

		if (s_x < img.rows)
			op_row = new shrinkCol;
		else if (s_x > img.rows)
			op_row = new enlargeCol;
		if (op_row)
		{
			Mat tr;
			transpose(buf, tr);
			Mat out(resizeImg(tr, s_x, gr2, op_row));
			transpose(out, output);
		}
		else
		{
			buf.copyTo(output);
		}
	}

    //compression
    vector<int> compression_params;
    compression_params.push_back(IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);

	string rslt = out_dir + src + string("_") + type + string(".png");
	imwrite(rslt, output, compression_params);
    printf("Output path: %s\n\n ", rslt.c_str());

    imshow("Result", output);
    cvWaitKey();

    if(gr)
        delete gr;
    if(gr2)
        delete gr2;
	if (rm)
		delete rm;
    if(op_col)
        delete op_col;
    if(op_row)
        delete op_row;

	return 0;
}
