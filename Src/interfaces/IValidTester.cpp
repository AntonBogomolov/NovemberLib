#include "../../Include/novemberlib/interfaces/IValidTester.h"

IValidTester::IValidTester()
{
	isValid = false;
}

IValidTester::~IValidTester()
{
	//dtor
}

void IValidTester::setIsValid(bool isValid)
{
	this->isValid = isValid;
}

bool IValidTester::getIsValid() const
{
	return isValid;
}
