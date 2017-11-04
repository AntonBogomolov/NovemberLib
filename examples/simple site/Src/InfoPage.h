<<<<<<< HEAD
#pragma once
#include "SitePage.h"

class CInfoPage :	public CSitePage
{
public:
	CInfoPage(const std::string name, const CFCGIRequest* currRequest);
	virtual ~CInfoPage();
protected:
	virtual std::string buildContent() const;
};

=======
#pragma once
#include "SitePage.h"

class CInfoPage :	public CSitePage
{
public:
	CInfoPage(const std::string name, const CFCGIRequest* currRequest);
	virtual ~CInfoPage();
protected:
	virtual std::string buildContent() const;
};

>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
