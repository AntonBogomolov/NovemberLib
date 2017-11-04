<<<<<<< HEAD
#pragma once
#include "SitePage.h"

class CIndexPage : public CSitePage
{
public:
	CIndexPage(const std::string name, const CFCGIRequest* currRequest);
	virtual ~CIndexPage();
protected:
	virtual std::string buildContent() const;
};
=======
#pragma once
#include "SitePage.h"

class CIndexPage : public CSitePage
{
public:
	CIndexPage(const std::string name, const CFCGIRequest* currRequest);
	virtual ~CIndexPage();
protected:
	virtual std::string buildContent() const;
};
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
