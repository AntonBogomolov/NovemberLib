#include "SitePage.h"


CSitePage::CSitePage(const std::string name, const CFCGIRequest* currRequest) : CPage(name, currRequest)
{

}

CSitePage::~CSitePage()
{

}

std::string CSitePage::buildLeftPanel()
{
	CTemplateHelper* templateManager = CTemplateHelper::getInstance();
	std::map<std::string, std::string> params;

	const CHTMLTemplate* leftPanelTemplate = templateManager->findTemplate("leftPanel");
	if (leftPanelTemplate == NULL) return "Missing menu template";

	std::string tmpString = "<table class='left_menu'>";
	tmpString += "<tr><td class='group'><span>Menu</span></td></tr>";
	tmpString += "<tr><td class='cat'><a href='?page=index'>Main Page</a></td></tr>";
	tmpString += "<tr><td class='cat'><a href='?page=info' >Info Page</a></td></tr>";
	tmpString += "</table>";

	params["{MENU}"] = tmpString;
	return  leftPanelTemplate->getHTMLData(&params);
}
