#ifndef APENG_H_
#define LAPENG_H_

#include "Energy.h"
class LapEng : public Energy
{
public:
	LapEng() {};
	~LapEng();
	void getEnergy(const Mat & src, Mat & dst);
};

#endif
