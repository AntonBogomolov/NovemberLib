#include "CSiteSessionManager.h"

#include "novemberlib/FCGI/CFCGIApp.h"
#include "novemberlib/FCGI/CFCGIRequest.h"
#include "novemberlib/helpers/CConfigHelper.h"
#include "novemberlib/managers/CManagers.h"
#include "novemberlib/managers/CDBManager.h"
#include "novemberlib/utils/CLog.h"

#include "CUser.h"

CSiteSessionManager::CSiteSessionManager()
{
	//ctor
}

CSiteSessionManager::~CSiteSessionManager()
{
	//dtor
}

bool CSiteSessionManager::checkSession(CFCGIRequest* currRequest)
{
	CDBManager* dbManager = CManagers::getInstance()->getDBManager();
	std::shared_ptr<CDBRequest> dbRequest(dbManager->createDBRequest());
	CConfigHelper* gs = CConfigHelper::getInstance();

	CLog::getInstance()->addInfo(currRequest->getRequestForModify()->locale());

	time_t currUnixTime;
	time(&currUnixTime);

	std::string cookieData = dbManager->getEscapeString(currRequest->getRequest()->cookie.get(gs->getStringParamValue("cookieName", "SESSIONID"),""));

	if(cookieData.length() == 0)							// new user
	{
		currRequest->getRequestForModify()->createCookie(gs->getStringParamValue("cookieName", "SESSIONID"), gs->getStringParamValue("guestCookieValue", "GUEST"), gs->getLongParamValue("sessionLifeTimeInMs", 5184000l));
		cookieData = gs->getStringParamValue("guestCookieValue", "GUEST");
	}
	else													// old user has cookie
	{
		const CDBRequestResult* result = dbRequest->selectRequest(CDBValues("id"), "Users", "`key` = '" + cookieData + "'", "LIMIT 1");
		if(dbRequest->getIsLastQuerySuccess() && result != NULL  && result->getRowsCnt() > 0)
		{
			currRequest->getRequestForModify()->createCookie(gs->getStringParamValue("cookieName", "SESSIONID"), cookieData, gs->getLongParamValue("sessionLifeTimeInMs", 5184000l));
		}
		else
		{
			std::string newSessionStr = dbManager->getEscapeString(CUser::genUserKey());
			currRequest->getRequestForModify()->createCookie(gs->getStringParamValue("cookieName", "SESSIONID"), newSessionStr, gs->getLongParamValue("sessionLifeTimeInMs", 5184000l));
			cookieData = newSessionStr;
			CUser::createNewUser(cookieData, currRequest->getRequest()->param.get("REMOTE_ADDR",""), currRequest->getRequest()->param.get("HTTP_USER_AGENT",""));
		}
	}

	CUser* currUser = new CUser();
	currUser->fillUserData(cookieData);
	currRequest->setUser(currUser);

	if(!currRequest->getUser()->getIsValid() || currRequest->getUser()->getIsAccessClosed()) return false;
	return true;
}
