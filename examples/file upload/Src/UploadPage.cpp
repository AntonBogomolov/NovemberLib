<<<<<<< HEAD
#include "UploadPage.h"
#include "SiteUser.h"

CUploadPage::CUploadPage(const std::string name, const CFCGIRequest* currRequest) : CSitePage(name, currRequest)
{

}

CUploadPage::~CUploadPage()
{

}

std::string CUploadPage::buildContent() const
{
	CTemplateHelper* templateManager = CTemplateHelper::getInstance();
	std::map<std::string, std::string> params;
	std::string tmpStr;

	const CHTMLTemplate* contentTemplate = templateManager->findTemplate("content");
	const CHTMLTemplate* uploadTemplate = templateManager->findTemplate("uploadPage");
	if (contentTemplate == NULL || uploadTemplate == NULL) return "Missing template";

	const CSiteUser* currUser = dynamic_cast<const CSiteUser*>(currRequest->getUser());
	if (!currUser || !currUser->getIsValid()) return CErrorPage("User not valid", "Sorry, but User not valid", "403").getPageHTMLContent();

	params["{ATTACHS}"] = buildAttachments(currUser->getAttachments());
	params["{USERLOGIN}"] = htmlspecialchars(currUser->getLogin());
	params["{USERID}"] = valueToString(currUser->getUserId());
	std::string uploadTemplateReadyContent = uploadTemplate->getHTMLData(&params);
	params.clear();

	params["{NAV}"] = buildNavBar(1);
	params["{CONTENT}"] = uploadTemplateReadyContent;

	return contentTemplate->getHTMLData(&params);
}
=======
#include "UploadPage.h"
#include "SiteUser.h"

CUploadPage::CUploadPage(const std::string name, const CFCGIRequest* currRequest) : CSitePage(name, currRequest)
{

}

CUploadPage::~CUploadPage()
{

}

std::string CUploadPage::buildContent() const
{
	CTemplateHelper* templateManager = CTemplateHelper::getInstance();
	std::map<std::string, std::string> params;
	std::string tmpStr;

	const CHTMLTemplate* contentTemplate = templateManager->findTemplate("content");
	const CHTMLTemplate* uploadTemplate = templateManager->findTemplate("uploadPage");
	if (contentTemplate == NULL || uploadTemplate == NULL) return "Missing template";

	const CSiteUser* currUser = dynamic_cast<const CSiteUser*>(currRequest->getUser());
	if (!currUser || !currUser->getIsValid()) return CErrorPage("User not valid", "Sorry, but User not valid", "403").getPageHTMLContent();

	params["{ATTACHS}"] = buildAttachments(currUser->getAttachments());
	params["{USERLOGIN}"] = htmlspecialchars(currUser->getLogin());
	params["{USERID}"] = valueToString(currUser->getUserId());
	std::string uploadTemplateReadyContent = uploadTemplate->getHTMLData(&params);
	params.clear();

	params["{NAV}"] = buildNavBar(1);
	params["{CONTENT}"] = uploadTemplateReadyContent;

	return contentTemplate->getHTMLData(&params);
}
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
