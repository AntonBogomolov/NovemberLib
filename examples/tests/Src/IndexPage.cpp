#include "IndexPage.h"

CIndexPage::CIndexPage(const std::string name, const CFCGIRequest* currRequest) : CSitePage(name, currRequest)
{

}

CIndexPage::~CIndexPage()
{

}

std::string CIndexPage::buildContent() const
{
	CTemplateHelper* templateManager = CTemplateHelper::getInstance();
	std::map<std::string, std::string> params;
	std::string tmpStr;

	const CHTMLTemplate* contentTemplate = templateManager->findTemplate("content");
	const CHTMLTemplate* indexTemplate = templateManager->findTemplate("indexPage");
	if (contentTemplate == NULL || indexTemplate == NULL) return "Missing template";

	std::string indexTemplateReadyContent = indexTemplate->getHTMLData(&params);
	params.clear();

	params["{NAV}"] = buildNavBar(0);
	params["{CONTENT}"] = indexTemplateReadyContent;

	return contentTemplate->getHTMLData(&params);
}
