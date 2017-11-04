<<<<<<< HEAD
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

	params["{TITLE}"] = "NovemberLib";
	params["{DESCRIPTION}"] = "fcgi c++ lib";
	std::string indexTemplateReadyContent = indexTemplate->getHTMLData(&params);
	params.clear();

	params["{LEFTPANEL}"] = buildLeftPanel();
	params["{CONTENT}"] = indexTemplateReadyContent;

	return contentTemplate->getHTMLData(&params);
}
=======
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

	params["{TITLE}"] = "NovemberLib";
	params["{DESCRIPTION}"] = "fcgi c++ lib";
	std::string indexTemplateReadyContent = indexTemplate->getHTMLData(&params);
	params.clear();

	params["{LEFTPANEL}"] = buildLeftPanel();
	params["{CONTENT}"] = indexTemplateReadyContent;

	return contentTemplate->getHTMLData(&params);
}
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
