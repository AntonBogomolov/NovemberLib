<<<<<<< HEAD
#pragma once
#include "SitePage.h"

class CUploadPage :	public CSitePage
{
public:
	CUploadPage(const std::string name, const CFCGIRequest* currRequest);
	virtual ~CUploadPage();
protected:
	virtual std::string buildContent() const;
};

=======
#pragma once
#include "SitePage.h"

class CUploadPage :	public CSitePage
{
public:
	CUploadPage(const std::string name, const CFCGIRequest* currRequest);
	virtual ~CUploadPage();
protected:
	virtual std::string buildContent() const;
};

>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
