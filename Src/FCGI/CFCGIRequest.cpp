#include "../../Include/novemberlib/FCGI/CFCGIRequest.h"

#include "../../Include/novemberlib/FCGI/CFCGIApp.h"

#include "../../Include/novemberlib/managers/CPageManager.h"
#include "../../Include/novemberlib/managers/CResourceManager.h"
#include "../../Include/novemberlib/managers/CCommandsManager.h"
#include "../../Include/novemberlib/managers/CSessionManager.h"
#include "../../Include/novemberlib/managers/CManagers.h"
#include "../../Include/novemberlib/helpers/CTextHelper.h"

#include "../../Include/novemberlib/utils/utils.h"
#include "../../Include/novemberlib/utils/CLog.h"
#include "../../Include/novemberlib/utils/CCodeTimer.h"

#include "../../Include/novemberlib/pages/CErrorPage.h"
#include "../../Include/novemberlib/CDefaultUser.h"

CFCGIRequest::CFCGIRequest(CFCGIRequestHandler* request)
{
	currRequest = request;
	userData = NULL;
<<<<<<< HEAD
	CLog::getInstance()->addInfo("+++++++ REQUEST CREATED +++++++");
=======

	CLog::getInstance()->addInfo("+++++++++++++++");
	CLog::getInstance()->addInfo("Request Created");
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
}

CFCGIRequest::~CFCGIRequest()
{
	delete userData;
<<<<<<< HEAD
	CLog::getInstance()->addInfo("+++++++ REQUEST DONE    +++++++");
=======

	CLog::getInstance()->addInfo("Request Deleted");
	CLog::getInstance()->addInfo("+++++++++++++++");
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
}

const CDefaultUser* CFCGIRequest::getUser() const
{
	return userData;
}

CDefaultUser* CFCGIRequest::getUserForModify()
{
	return userData;
}

void CFCGIRequest::setUser(CDefaultUser* newUser)
{
	if(newUser && newUser->getIsValid() && newUser != userData)
	{
		delete userData;
		userData = newUser;
	}
}

bool CFCGIRequest::response()
{
	std::string uri = currRequest->uri();
<<<<<<< HEAD
	std::string logMessage = "Start Request: ";
	logMessage += "[";
	logMessage += uri;
	logMessage += "] [";
	logMessage += currRequest->param.get("REMOTE_ADDR","");
	logMessage += "]";
	CLog::getInstance()->addInfo(logMessage);
=======

	CLog* log = CLog::getInstance();
	log->addInfo("Start Request");
	log->addInfo(uri);
	log->addInfo(currRequest->param.get("REMOTE_ADDR",""));
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311

	CResourceManager* resourceManager = CManagers::getInstance()->getResourceManager();
	CSessionManager*  sessionManager  = CManagers::getInstance()->getSessionManager();
	CCommandsManager* commandManager  = CManagers::getInstance()->getCommandManager();
	CPageManager* 	  pageManager  	  = CManagers::getInstance()->getPageManager();
<<<<<<< HEAD

=======
		
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
	// resources ////////////////////////////////
	if(resourceManager->getIsNeedProcessResourceRequest())
	{
		CResourceResult resourceResult = resourceManager->processResourceRequest(this);
		if(resourceResult.getIsValid())
		{
			if(!resourceResult.getIsSuccess())
			{
				currRequest->header.set("Content-Type", "text/html; charset=utf-8");
				currRequest->response << pageManager->getErrorPageContent("403", resourceResult.getMessage());
			}
			else
			{
				currRequest->header.set("Cache-Control", resourceResult.getCacheControlStr());
				currRequest->header.set("Etag", resourceResult.getEtag());
				currRequest->header.set("Content-Length", resourceResult.getDataSize());
				currRequest->header.set("Content-Type", resourceResult.getMIME());
				currRequest->response.write(resourceResult.getData(), resourceResult.getDataSize());
			}
			return true;
		}
<<<<<<< HEAD
	}
=======
	}	
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
	// sessions ///////////////////////////////////
	if(sessionManager->getIsNeedSessionCheck())
	{
		if(!sessionManager->checkSession(this))
		{
			currRequest->header.set("Content-Type", "text/html; charset=utf-8");
			currRequest->response << pageManager->getErrorPageContent("403", "User session error");
			return true;
		}
<<<<<<< HEAD
	}
=======
	}	
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
	// commands ///////////////////////////////////
	if(commandManager->getIsNeedProcessCommands())
	{
		CCommandResult commandResult = commandManager->processCommand(this);
		if(commandResult.getIsValid())
		{
			if(!commandResult.getIsSuccess())
			{
				currRequest->header.set("Content-Type", "text/html; charset=utf-8");
				currRequest->response << pageManager->getErrorPageContent("403", commandResult.getData());
			}
			else
			{
				currRequest->header.set("Content-Type", commandResult.getHeader());
<<<<<<< HEAD
				currRequest->response.write(commandResult.getData(), commandResult.getDataSize());
			}
			return true;
		}
	}
=======
				currRequest->response << commandResult.getData();
			}
			return true;
		}
	}	
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
	// pages /////////////////////////////////////
	if(pageManager->getIsNeedProcessPages())
	{
		CCodeTimer codeTimer;
<<<<<<< HEAD
		std::string page = currRequest->get.get("page", "index");
=======
		std::string page = currRequest->get.get("page", "index");		
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
		currRequest->header.set("Content-Type", "text/html; charset=utf-8");
		currRequest->response << CManagers::getInstance()->getPageManager()->getPageContent(page, this);
		currRequest->response << "<!-- Page generation time: " << codeTimer.getSpentTime() * 1000 << " ms -->";

		return true;
	}
<<<<<<< HEAD

=======
	
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
	currRequest->header.set("Content-Type", "text/html; charset=utf-8");
	currRequest->response << pageManager->getErrorPageContent("404", "Internal Server Error :'(");
    return true;
}

CFCGIRequestHandler* CFCGIRequest::getRequestForModify()
{
	return currRequest;
}

const CFCGIRequestHandler* CFCGIRequest::getRequest() const
{
	return currRequest;
}
