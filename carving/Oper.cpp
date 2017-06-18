#include "Oper.h"

Oper::Oper()
{
    //ctor
}

Oper::~Oper()
{
    //dtor
}

void Oper::calcDif(const Mat & src, Mat & grad, Energy* en)
{
    en->getEnergy(src, grad);
	return;
}
