<<<<<<< HEAD
#include "../../Include/novemberlib/managers/CDefaultCommandManager.h"

#include "../../Include/novemberlib/utils/utils.h"
#include "../../Include/novemberlib/FCGI/CFCGIRequest.h"
#include "../../Include/novemberlib/CDefaultUser.h"
#include "../../Include/novemberlib/helpers/CConfigHelper.h"
#include "../../Include/novemberlib/managers/CManagers.h"

#include <string>

CDefaultCommandManager::CDefaultCommandManager()
{
	//ctor
}

CDefaultCommandManager::~CDefaultCommandManager()
{
	//dtor
}


CCommandResult CDefaultCommandManager::processCommand(CFCGIRequest* currRequest)
{
	CFCGIRequestHandler* request  = currRequest->getRequestForModify();
	const std::string command = request->get.get("command","");
	CCommandResult result;
	result.setData("Not valid user");
	if (command.size() == (size_t)0)
	{
		result.setIsValid(false);
		result.setData("Not valid command");
		return result;
	}

	if(!currRequest->getUser() || !currRequest->getUser()->getIsValid())
	{
		result.setIsValid(false);
		result.setData("Not valid command");
		return result;
	}

    if(command == "about") return aboutCommand(currRequest);

	result.setIsValid(false);
	result.setData("Not valid command");
	return result;
}

CCommandResult CDefaultCommandManager::aboutCommand(CFCGIRequest* currRequest) const
{
	CCommandResult result;

	if(!currRequest->getUser() || currRequest->getUser()->getIsValid())
	{
		result.setData("Not valid user");
		return result;
	}

	result.setIsSuccess(true);
	result.setData("hi!");
    return result;
}

void CDefaultCommandManager::guestUserCheck(CFCGIRequest* currRequest) const
{
	if (!currRequest->getUser()) return;

    if(!currRequest->getUser()->getIsValid()) return;
	CConfigHelper* gs = CConfigHelper::getInstance();
    if(currRequest->getUser()->getCookie() == gs->getStringParamValue("guestCookieValue", "GUEST"))
    {
        std::string userKey = CDefaultUser::genUserKey();
        currRequest->getRequestForModify()->createCookie(gs->getStringParamValue("cookieName", "SESSIONID"), userKey, gs->getLongParamValue("sessionLifeTimeInMs", 5184000l));
        currRequest->setUser(CDefaultUser::createNewUser("","",userKey,valueToString(currRequest->getRequest()->param.get("REMOTE_ADDR","")), currRequest->getRequest()->param.get("HTTP_USER_AGENT","") ));
    }
}
=======
#include "../../Include/novemberlib/managers/CDefaultCommandManager.h"

#include "../../Include/novemberlib/utils/utils.h"
#include "../../Include/novemberlib/FCGI/CFCGIRequest.h"
#include "../../Include/novemberlib/CDefaultUser.h"
#include "../../Include/novemberlib/helpers/CConfigHelper.h"
#include "../../Include/novemberlib/managers/CManagers.h"

#include <string>

CDefaultCommandManager::CDefaultCommandManager()
{
	//ctor
}

CDefaultCommandManager::~CDefaultCommandManager()
{
	//dtor
}


CCommandResult CDefaultCommandManager::processCommand(CFCGIRequest* currRequest)
{
	CFCGIRequestHandler* request  = currRequest->getRequestForModify();
	const std::string command = request->get.get("command","");
	CCommandResult result;
	result.setData("Not valid user");
	if (command.size() == (size_t)0)
	{
		result.setIsValid(false);
		result.setData("Not valid command");
		return result;
	}

	if(!currRequest->getUser() || !currRequest->getUser()->getIsValid())
	{
		result.setIsValid(false);
		result.setData("Not valid command");
		return result;
	}

    if(command == "about") return aboutCommand(currRequest);

	result.setIsValid(false);
	result.setData("Not valid command");
	return result;
}

CCommandResult CDefaultCommandManager::aboutCommand(CFCGIRequest* currRequest) const
{
	CCommandResult result;

	if(!currRequest->getUser() || currRequest->getUser()->getIsValid())
	{
		result.setData("Not valid user");
		return result;
	}

	result.setIsSuccess(true);
	result.setData("hi!");
    return result;
}

void CDefaultCommandManager::guestUserCheck(CFCGIRequest* currRequest) const
{
	if (!currRequest->getUser()) return;

    if(!currRequest->getUser()->getIsValid()) return;
	CConfigHelper* gs = CConfigHelper::getInstance();
    if(currRequest->getUser()->getCookie() == gs->getStringParamValue("guestCookieValue", "GUEST"))
    {
        std::string userKey = CDefaultUser::genUserKey();
        currRequest->getRequestForModify()->createCookie(gs->getStringParamValue("cookieName", "SESSIONID"), userKey, gs->getLongParamValue("sessionLifeTimeInMs", 5184000l));
        currRequest->setUser(CDefaultUser::createNewUser("","",userKey,valueToString(currRequest->getRequest()->param.get("REMOTE_ADDR","")), currRequest->getRequest()->param.get("HTTP_USER_AGENT","") ));
    }
}
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
