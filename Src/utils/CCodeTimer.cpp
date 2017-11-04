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
<<<<<<< HEAD
	double result = (double)(end - start) / CLOCKS_PER_SEC;
	start = clock();

	return result;
=======
	return (double)(end - start) / CLOCKS_PER_SEC;
	start = clock();
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
}
