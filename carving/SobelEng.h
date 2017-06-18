#ifndef SOBELENG_H_
#define SOBELENG_H_

#include "Energy.h"
class SobelEng : public Energy
{
public:
	SobelEng() {};
	~SobelEng();

	void getEnergy(const Mat& src, Mat & dst);
};

#endif
