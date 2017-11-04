<<<<<<< HEAD
#pragma once
#include "novemberlib/novemberlib.h"

class CSitePage : public CPage
{
public:
	static std::string buildLeftPanel();

	CSitePage(const std::string name, const CFCGIRequest* currRequest);
	virtual ~CSitePage();
};
=======
#pragma once
#include "novemberlib/novemberlib.h"

class CSitePage : public CPage
{
public:
	static std::string buildLeftPanel();

	CSitePage(const std::string name, const CFCGIRequest* currRequest);
	virtual ~CSitePage();
};
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
