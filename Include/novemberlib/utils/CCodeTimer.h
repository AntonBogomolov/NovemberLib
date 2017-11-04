#ifndef CCODETIMER_H
#define CCODETIMER_H

#include <time.h>

class CCodeTimer
{
	public:
		double getSpentTime() const;
		double reset();

		CCodeTimer();
		~CCodeTimer();
	protected:
	private:
		clock_t start;
};

#endif // CCODETIMER_H
