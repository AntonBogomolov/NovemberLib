#include "../../Include/novemberlib/pages/CPage.h"

#include "../../Include/novemberlib/utils/utils.h"
#include "../../Include/novemberlib/FCGI/CFCGIRequest.h"
#include "../../Include/novemberlib/managers/CManagers.h"
#include "../../Include/novemberlib/helpers/CTemplateHelper.h"
#include "../../Include/novemberlib/CHTMLTemplate.h"
#include "../../Include/novemberlib/utils/CLog.h"

CPage::CPage(const std::string& name, const CFCGIRequest* currRequest)
{
	this->pageName 	= name;
	this->currRequest = currRequest;
	isPageBuilded = false;
}

CPage::~CPage()
{
	//dtor
}

const std::string CPage::getPageName() const
{
	return pageName;
}

const std::string CPage::getPageHTMLContent() const
{
	if(!isPageBuilded) buildPage();

	return pageHTMLContent;
}

void CPage::buildPage() const
{
	pageHTMLContent = "";
	pageHTMLContent += buildHeader();
    pageHTMLContent += buildContent();
	pageHTMLContent += buildFooter();

	isPageBuilded = true;
}

std::string CPage::buildHeader() const
{
	CTemplateHelper* templateManager = CTemplateHelper::getInstance();

	const CHTMLTemplate* headerTemplate = templateManager->findTemplate("header");
	if(headerTemplate ==  NULL) return "Missing header template";

	return headerTemplate->getHTMLData();
}

std::string CPage::buildFooter() const
{
	CTemplateHelper* templateManager = CTemplateHelper::getInstance();

	const CHTMLTemplate* footerTemplate = templateManager->findTemplate("footer");
	if(footerTemplate == NULL) return "Missing footer template";

	return footerTemplate->getHTMLData();
}
