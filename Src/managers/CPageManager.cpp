#include "../../Include/novemberlib/managers/CPageManager.h"

#include "../../Include/novemberlib/FCGI/CFCGIRequest.h"
#include "../../Include/novemberlib/pages/CErrorPage.h"
#include "../../Include/novemberlib/CDefaultUser.h"
#include "../../Include/novemberlib/utils/CLog.h"
#include "../../Include/novemberlib/helpers/CSyncHelper.h"

CPageManager::CPageManager()
{
    errorPage = NULL;
    isNeedProcessPages = true;
}

CPageManager::~CPageManager()
{
    delete errorPage;
}

bool CPageManager::getIsNeedProcessPages() const
{
    return isNeedProcessPages;
}

void CPageManager::setIsNeedProcessPages(const bool value)
{
    isNeedProcessPages = value;
}

const std::string CPageManager::getPageContent(const std::string& pageName, const CFCGIRequest* currRequest) const
{
    if(!isNeedProcessPages) return "Page manager not working";

    std::string pageContent = "";
    CLog::getInstance()->addInfo("Page request: " + pageName);

	CPage* page = createPage(pageName, currRequest);      
    // 404 Error
    if(!page)
    {
        pageContent = getErrorPageContent("404", "Sorry, resource not found :'(");
        return pageContent;
    }

	pageContent = page->getPageHTMLContent();
	delete page;

    return pageContent;
}

void CPageManager::setErrorPage(CErrorPage* errorPage)
{
    if(errorPage && errorPage != this->errorPage)
    {
        delete this->errorPage;
        this->errorPage = errorPage;
    }
}

const std::string CPageManager::getErrorPageContent(const std::string& errorCode, const std::string& errorMessage) const
{
    std::string content;
    CSyncHelper::getInstance()->getPageMutex()->lock();

    if(errorPage == NULL) errorPage = new CErrorPage("errorPage", errorMessage, errorCode);
    errorPage->setErrorCode(errorCode);
    errorPage->setErrorMessage(errorMessage);
    content = errorPage->getPageHTMLContent();

    CSyncHelper::getInstance()->getPageMutex()->unlock();
    return content;
}
