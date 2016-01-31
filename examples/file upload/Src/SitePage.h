#pragma once
#include "novemberlib/novemberlib.h"

class CAttachment;

class CSitePage : public CPage
{
public:
	static std::string buildNavBar(const int currCat = 0);
	static std::string buildAttachments(const std::vector<CAttachment*>* attachments);

	CSitePage(const std::string name, const CFCGIRequest* currRequest);
	virtual ~CSitePage();
};
