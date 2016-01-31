#include "../../Include/novemberlib/utils/CCodeTimer.h"

CCodeTimer::CCodeTimer()
{
	start = clock();
}

CCodeTimer::~CCodeTimer()
{
	//dtor
}

double CCodeTimer::getSpentTime() const
{
	clock_t end = clock();
	return (double)(end - start) / CLOCKS_PER_SEC;
}

double CCodeTimer::reset()
{
	clock_t end = clock();
	return (double)(end - start) / CLOCKS_PER_SEC;
	start = clock();
}
