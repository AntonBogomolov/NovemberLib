#pragma once
#include "novemberlib/novemberlib.h"

class CApp : public CFCGIApp
{
public:
	CApp();
	virtual ~CApp();
protected:
	virtual void init();
};

