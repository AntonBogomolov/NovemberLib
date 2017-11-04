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
<<<<<<< HEAD
	userCreationFunc = nullptr;
=======
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
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
<<<<<<< HEAD
	const bool isHasDB = gs->getStringParamValue("isHasDB", "false") == "true";
	const std::string guestCookieValue  = gs->getStringParamValue("guestCookieValue", "GUEST");
	const std::string cookieName        = gs->getStringParamValue("cookieName", "SESSIONID");
	const long cookieLifeTime           = gs->getLongParamValue("sessionLifeTimeInMs", 5184000l);

	if (!isNeedSessionCheck || !isHasDB)
	{
		CDefaultUser* currUser = userCreationFunc();
		currUser->fillUserData(guestCookieValue);
=======
	bool isHasDB = gs->getStringParamValue("isHasDB", "false") == "true";
	if (!isNeedSessionCheck || !isHasDB)
	{
		CDefaultUser* currUser = userCreationFunc();
		currUser->fillUserData(gs->getStringParamValue("guestCookieValue", "GUEST"));
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
		currRequest->setUser(currUser);
		return true;
	}
	time_t currUnixTime;
	time(&currUnixTime);

<<<<<<< HEAD
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
=======
	std::string cookieData = currRequest->getRequest()->cookie.get(gs->getStringParamValue("cookieName", "SESSIONID"), "");

	if(cookieData.length() == 0)							// new user
	{
		currRequest->getRequestForModify()->createCookie(gs->getStringParamValue("cookieName", "SESSIONID"), gs->getStringParamValue("guestCookieValue", "GUEST"), gs->getLongParamValue("sessionLifeTimeInMs", 5184000l));
		cookieData = gs->getStringParamValue("guestCookieValue", "GUEST");
	}
	else													// old user has cookie
	{		
		if(!isHasDB)
		{
			cookieData = gs->getStringParamValue("guestCookieValue", "GUEST");
		}

		const bool isUserExist = CDefaultUser::isUserExist(cookieData);
		if(isUserExist)
		{
			currRequest->getRequestForModify()->createCookie(gs->getStringParamValue("cookieName", "SESSIONID"), cookieData, gs->getLongParamValue("sessionLifeTimeInMs", 5184000l));
		}
		else
		{
			currRequest->getRequestForModify()->createCookie(gs->getStringParamValue("cookieName", "SESSIONID"), gs->getStringParamValue("guestCookieValue", "GUEST"), gs->getLongParamValue("sessionLifeTimeInMs", 5184000l));
			cookieData = gs->getStringParamValue("guestCookieValue", "GUEST");
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
		}
	}

	CDefaultUser* currUser = userCreationFunc();
	currUser->fillUserData(cookieData);
	currRequest->setUser(currUser);

	if(!currRequest->getUser()->getIsValid() || currRequest->getUser()->getIsAccessClosed()) return false;
	return true;
}

void CSessionManager::loginUser(CFCGIRequest* currRequest, const std::string& login, const std::string& pass)
<<<<<<< HEAD
{
	CConfigHelper* gs = CConfigHelper::getInstance();

	CDefaultUser* currUser = CDefaultUser::loginUser(login, pass);
	if (currUser && currUser->getIsValid())
	{
=======
{	
	CConfigHelper* gs = CConfigHelper::getInstance();
		
	CDefaultUser* currUser = CDefaultUser::loginUser(login, pass);
	if (currUser && currUser->getIsValid())
	{		
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
		currRequest->getRequestForModify()->createCookie(gs->getStringParamValue("cookieName", "SESSIONID"), currUser->getCookie(), gs->getLongParamValue("sessionLifeTimeInMs", 5184000l));
		currRequest->setUser(currUser);
	}
	else
	{
		currRequest->getRequestForModify()->createCookie(gs->getStringParamValue("cookieName", "SESSIONID"), gs->getStringParamValue("guestCookieValue", "GUEST"), gs->getLongParamValue("sessionLifeTimeInMs", 5184000l));
	}
}

<<<<<<< HEAD
void CSessionManager::logoutUser(CFCGIRequest* currRequest)
{
    CConfigHelper* gs = CConfigHelper::getInstance();
    currRequest->getRequestForModify()->createCookie(gs->getStringParamValue("cookieName", "SESSIONID"), gs->getStringParamValue("guestCookieValue", "GUEST"), gs->getLongParamValue("sessionLifeTimeInMs", 5184000l));
}

=======
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
void CSessionManager::registerUser(CFCGIRequest* currRequest, const std::string& login, const std::string& pass)
{
	CDBManager* dbManager = CManagers::getInstance()->getDBManager();
	CConfigHelper* gs = CConfigHelper::getInstance();
<<<<<<< HEAD

=======
	
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
	std::string newSessionStr = dbManager->getEscapeString(CDefaultUser::genUserKey());
	currRequest->getRequestForModify()->createCookie(gs->getStringParamValue("cookieName", "SESSIONID"), newSessionStr, gs->getLongParamValue("sessionLifeTimeInMs", 5184000l));
	std::string cookieData = newSessionStr;

	CDefaultUser* currUser = CDefaultUser::createNewUser(login, pass, cookieData, currRequest->getRequest()->param.get("REMOTE_ADDR",""), currRequest->getRequest()->param.get("HTTP_USER_AGENT",""));
	if (currUser->getIsValid())
	{
<<<<<<< HEAD
		currRequest->setUser(currUser);
	}
	else
	{
=======
		currRequest->setUser(currUser);		
	}
	else
	{		
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
		currRequest->getRequestForModify()->createCookie(gs->getStringParamValue("cookieName", "SESSIONID"), gs->getStringParamValue("guestCookieValue", "GUEST"), gs->getLongParamValue("sessionLifeTimeInMs", 5184000l));
	}
}
