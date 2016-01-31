#include "InfoPage.h"

CInfoPage::CInfoPage(const std::string name, const CFCGIRequest* currRequest) : CSitePage(name, currRequest)
{

}

CInfoPage::~CInfoPage()
{

}

std::string CInfoPage::buildContent() const
{
	CTemplateHelper* templateManager = CTemplateHelper::getInstance();
	std::map<std::string, std::string> params;
	std::string tmpStr;

	const CHTMLTemplate* contentTemplate = templateManager->findTemplate("content");
	const CHTMLTemplate* infoTemplate = templateManager->findTemplate("infoPage");
	if (contentTemplate == NULL || infoTemplate == NULL) return "Missing template";

	params["{TITLE}"] = "NovemberLib";
	std::string indexTemplateReadyContent = infoTemplate->getHTMLData(&params);
	params.clear();

	params["{LEFTPANEL}"] = buildLeftPanel();
	params["{CONTENT}"] = indexTemplateReadyContent;

	return contentTemplate->getHTMLData(&params);
}
