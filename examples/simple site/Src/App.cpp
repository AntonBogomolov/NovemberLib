#include "App.h"

#include "novemberlib/FCGI/CFCGIApp.h"
#include "novemberlib/FCGI/CFCGIRequest.h"
#include "novemberlib/managers/CManagers.h"
#include "novemberlib/managers/CPageManager.h"
#include "novemberlib/managers/CCommandsManager.h"
#include "novemberlib/helpers/CPathsHelper.h"
#include "novemberlib/helpers/CTemplateHelper.h"
#include "novemberlib/pages/CPage.h"

#include "SiteCommandsManager.h"
#include "IndexPage.h"
#include "InfoPage.h"

CApp::CApp()
{
	CManagers::getInstance()->setCommandManager(new CSiteCommandsManager());
}

CApp::~CApp()
{

}

void CApp::init()
{
	CFCGIApp::init();

	CPathsHelper* paths = CPathsHelper::getInstance();
	paths->addPath("", "*/*", true);
	paths->addPath("templates", "*/*", true);

	CTemplateHelper* templates = CTemplateHelper::getInstance();
	const CPath* templatesDir = paths->getPath("templates");
	templates->loadTemplate(templatesDir, "header.html", "header");
	templates->loadTemplate(templatesDir, "footer.html", "footer");
	templates->loadTemplate(templatesDir, "content.html", "content");
	templates->loadTemplate(templatesDir, "leftPanel.html", "leftPanel");
	templates->loadTemplate(templatesDir, "indexPage.html", "indexPage");
	templates->loadTemplate(templatesDir, "infoPage.html", "infoPage");

	CPageManager* pages = CManagers::getInstance()->getPageManager();
	pages->addPageType<CIndexPage>("index");
	pages->addPageType<CInfoPage>("info");
}
