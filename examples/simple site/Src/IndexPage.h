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
