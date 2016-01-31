#include "../../Include/novemberlib/managers/CManagers.h"

#include "../../Include/novemberlib/managers/CPageManager.h"
#include "../../Include/novemberlib/managers/CSessionManager.h"
#include "../../Include/novemberlib/managers/CCommandsManager.h"
#include "../../Include/novemberlib/managers/CResourceManager.h"
#include "../../Include/novemberlib/managers/CDBManager.h"

CManagers::CManagers()
{
	pageManager = NULL;
	sessionManager  = NULL;
	commandManager  = NULL;
	resourceManager = NULL;
	dbManager 		= NULL;
	isLocked = false;
}

CManagers::~CManagers()
{
	delete pageManager;
	delete sessionManager;
	delete commandManager;
	delete resourceManager;
	delete dbManager;
}

CPageManager* CManagers::getPageManager() const
{
	return pageManager;
}

CSessionManager* CManagers::getSessionManager() const
{
	return sessionManager;
}

CCommandsManager* CManagers::getCommandManager() const
{
	return commandManager;
}

CResourceManager* CManagers::getResourceManager() const
{
	return resourceManager;
}

CDBManager* CManagers::getDBManager() const
{
	return dbManager;
}

void CManagers::lock()
{
	isLocked = true;
}

void CManagers::setPageManager(CPageManager* pageManager)
{
	if(!isLocked && pageManager && pageManager != this->pageManager)
	{
		delete this->pageManager;
		this->pageManager = pageManager;
	}
}

void CManagers::setSessionManager(CSessionManager* sessionManager)
{
	if(!isLocked && sessionManager && sessionManager != this->sessionManager)
	{
		delete this->sessionManager;
		this->sessionManager = sessionManager;
	}
}

void CManagers::setCommandManager(CCommandsManager* commandManager)
{
	if(!isLocked && commandManager && commandManager != this->commandManager)
	{
		delete this->commandManager;
		this->commandManager = commandManager;
	}
}

void CManagers::setResourceManager(CResourceManager* resourceManager)
{
	if(!isLocked && resourceManager && resourceManager != this->resourceManager)
	{
		delete this->resourceManager;
		this->resourceManager = resourceManager;
	}
}

void CManagers::setDBManager(CDBManager* dbManager)
{
	if(!isLocked && dbManager && dbManager != this->dbManager)
	{
		delete this->dbManager;
		this->dbManager = dbManager;
	}
}
