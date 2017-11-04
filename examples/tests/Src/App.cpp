#include "App.h"

#include "SiteUser.h"
#include "SiteCommandsManager.h"
#include "IndexPage.h"
#include "UploadPage.h"

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

	CManagers::getInstance()->getSessionManager()->setUserType<CSiteUser>();

	CPathsHelper* paths = CPathsHelper::getInstance();
	// SITE CONTENT PATHS
	paths->addPath("css", "*/*", true);
	paths->addPath("fonts", "*/*", true);
	paths->addPath("img", "image/*", true);
	paths->addPath("img/previewicons", "image/*", true);
	paths->addPath("js", "text/javascript", true);
	paths->addPath("templates", "*/*", true);
	paths->addPath("", "image/vnd.microsoft.icon", true);
	// USER CONTENT PATHS	
	paths->addPath("aud", "audio/*");
	paths->addPath("img", "image/*");
	paths->addPath("others", "*/*");

	CTemplateHelper* templates = CTemplateHelper::getInstance();
	const CPath* templatesDir = paths->getPath("templates");
	templates->loadTemplate(templatesDir, "header.html", "header");
	templates->loadTemplate(templatesDir, "footer.html", "footer");
	templates->loadTemplate(templatesDir, "content.html", "content");
	templates->loadTemplate(templatesDir, "nav.html", "navBar");
	templates->loadTemplate(templatesDir, "attach.html", "attach");
	templates->loadTemplate(templatesDir, "leftPanel.html", "leftPanel");
	templates->loadTemplate(templatesDir, "indexPage.html", "indexPage");
	templates->loadTemplate(templatesDir, "uploadPage.html", "uploadPage");

	CPageManager* pages = CManagers::getInstance()->getPageManager();
	pages->addPageType<CIndexPage>("index");
	pages->addPageType<CUploadPage>("upload");
}
