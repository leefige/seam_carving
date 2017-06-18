#ifndef ROBENG_H_
#define ROBENG_H_

#include "Energy.h"
class RobEng : public Energy
{
public:
	RobEng() {};
	~RobEng();

	void getEnergy(const Mat& src, Mat & dst);
};

#endif
