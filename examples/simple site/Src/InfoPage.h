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

