#ifndef IVALIDTESTER_H
#define IVALIDTESTER_H

#include "../utils/config.h"

class IValidTester
{
	public:
		virtual bool getIsValid() const;

		IValidTester();
		virtual ~IValidTester() = 0;
	protected:
		bool isValid;
		virtual void setIsValid(bool isValid);
	private:

};

#endif // IVALIDTESTER_H
