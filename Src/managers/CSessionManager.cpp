#include "../../Include/novemberlib/managers/CSessionManager.h"

#include "../../Include/novemberlib/managers/CManagers.h"
#include "../../Include/novemberlib/managers/CDBManager.h"
#include "../../Include/novemberlib/utils/utils.h"
#include "../../Include/novemberlib/utils/CLog.h"

#include "../../Include/novemberlib/CDefaultUser.h"
#include "../../Include/novemberlib/helpers/CConfigHelper.h"
#include "../../Include/novemberlib/managers/CManagers.h"
#include "../../Include/novemberlib/FCGI/CFCGIRequest.h"

CSessionManager::CSessionManager()
{
	isNeedSessionCheck = true;
	userCreationFunc = nullptr;
	setUserType<CDefaultUser>();
}

CSessionManager::~CSessionManager()
{
	//dtor
}

bool CSessionManager::getIsNeedSessionCheck() const
{
	return isNeedSessionCheck;
}

void CSessionManager::setIsNeedSessionCheck(const bool value)
{
	isNeedSessionCheck = value;
}

bool CSessionManager::checkSession(CFCGIRequest* currRequest)
{
	CConfigHelper* gs = CConfigHelper::getInstance();
	const bool isHasDB = gs->getStringParamValue("isHasDB", "false") == "true";
	const std::string guestCookieValue  = gs->getStringParamValue("guestCookieValue", "GUEST");
	const std::string cookieName        = gs->getStringParamValue("cookieName", "SESSIONID");
	const long cookieLifeTime           = gs->getLongParamValue("sessionLifeTimeInMs", 5184000l);

	if (!isNeedSessionCheck || !isHasDB)
	{
		CDefaultUser* currUser = userCreationFunc();
		currUser->fillUserData(guestCookieValue);
		currRequest->setUser(currUser);
		return true;
	}
	time_t currUnixTime;
	time(&currUnixTime);

	std::string cookieData = currRequest->getRequest()->cookie.get(cookieName, "");

	if(cookieData.length() == 0)							// new user
	{
		currRequest->getRequestForModify()->createCookie(cookieName, guestCookieValue, cookieLifeTime);
		cookieData = guestCookieValue;
	}
	else													// old user has cookie
	{
		if(!isHasDB || cookieData == guestCookieValue)
		{
			currRequest->getRequestForModify()->createCookie(cookieName, guestCookieValue, cookieLifeTime);
			cookieData = guestCookieValue;
		}
		else
		if(CDefaultUser::isUserExist(cookieData))
		{
			currRequest->getRequestForModify()->createCookie(cookieName, cookieData, cookieLifeTime);
		}
	}

	CDefaultUser* currUser = userCreationFunc();
	currUser->fillUserData(cookieData);
	currRequest->setUser(currUser);

	if(!currRequest->getUser()->getIsValid() || currRequest->getUser()->getIsAccessClosed()) return false;
	return true;
}

void CSessionManager::loginUser(CFCGIRequest* currRequest, const std::string& login, const std::string& pass)
{
	CConfigHelper* gs = CConfigHelper::getInstance();

	CDefaultUser* currUser = CDefaultUser::loginUser(login, pass);
	if (currUser && currUser->getIsValid())
	{
		currRequest->getRequestForModify()->createCookie(gs->getStringParamValue("cookieName", "SESSIONID"), currUser->getCookie(), gs->getLongParamValue("sessionLifeTimeInMs", 5184000l));
		currRequest->setUser(currUser);
	}
	else
	{
		currRequest->getRequestForModify()->createCookie(gs->getStringParamValue("cookieName", "SESSIONID"), gs->getStringParamValue("guestCookieValue", "GUEST"), gs->getLongParamValue("sessionLifeTimeInMs", 5184000l));
	}
}

void CSessionManager::logoutUser(CFCGIRequest* currRequest)
{
    CConfigHelper* gs = CConfigHelper::getInstance();
    currRequest->getRequestForModify()->createCookie(gs->getStringParamValue("cookieName", "SESSIONID"), gs->getStringParamValue("guestCookieValue", "GUEST"), gs->getLongParamValue("sessionLifeTimeInMs", 5184000l));
}

void CSessionManager::registerUser(CFCGIRequest* currRequest, const std::string& login, const std::string& pass)
{
	CDBManager* dbManager = CManagers::getInstance()->getDBManager();
	CConfigHelper* gs = CConfigHelper::getInstance();

	std::string newSessionStr = dbManager->getEscapeString(CDefaultUser::genUserKey());
	currRequest->getRequestForModify()->createCookie(gs->getStringParamValue("cookieName", "SESSIONID"), newSessionStr, gs->getLongParamValue("sessionLifeTimeInMs", 5184000l));
	std::string cookieData = newSessionStr;

	CDefaultUser* currUser = CDefaultUser::createNewUser(login, pass, cookieData, currRequest->getRequest()->param.get("REMOTE_ADDR",""), currRequest->getRequest()->param.get("HTTP_USER_AGENT",""));
	if (currUser->getIsValid())
	{
		currRequest->setUser(currUser);
	}
	else
	{
		currRequest->getRequestForModify()->createCookie(gs->getStringParamValue("cookieName", "SESSIONID"), gs->getStringParamValue("guestCookieValue", "GUEST"), gs->getLongParamValue("sessionLifeTimeInMs", 5184000l));
	}
}
