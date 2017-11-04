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

