#pragma once
#include "novemberlib/novemberlib.h"

class CSitePage : public CPage
{
public:
	static std::string buildLeftPanel();

	CSitePage(const std::string name, const CFCGIRequest* currRequest);
	virtual ~CSitePage();
};
