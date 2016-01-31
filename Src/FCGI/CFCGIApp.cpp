#include "../../Include/novemberlib/FCGI/CFCGIApp.h"
#include "../../Include/novemberlib/FCGI/CFCGIRequest.h"
#include "../../Include/novemberlib/FCGI/CFCGIRequestHandler.h"

#include "../../Include/novemberlib/utils/utils.h"

#include "../../Include/novemberlib/managers/CDefaultCommandManager.h"
#include "../../Include/novemberlib/managers/CResourceManager.h"
#include "../../Include/novemberlib/managers/CMySQLDBManager.h"
#include "../../Include/novemberlib/managers/CPageManager.h"
#include "../../Include/novemberlib/managers/CSessionManager.h"
#include "../../Include/novemberlib/managers/CManagers.h"

#include "../../Include/novemberlib/helpers/CPathsHelper.h"
#include "../../Include/novemberlib/helpers/CConfigHelper.h"
#include "../../Include/novemberlib/helpers/CSyncHelper.h"
#include "../../Include/novemberlib/helpers/CTemplateHelper.h"
#include "../../Include/novemberlib/helpers/CTextHelper.h"

#include "../../Include/novemberlib/utils/CLog.h"
#include "../../Include/novemberlib/helpers/CMessageParser.h"

#include <vector>
#include <string>
#include <thread>

CFCGIApp::CFCGIApp()
{
	listenSocket = 0;

	CSyncHelper::getInstance();
	CConfigHelper::getInstance();
	CMessageParser::getInstance();
	CTemplateHelper::getInstance();
	CTextHelper::getInstance();
}

CFCGIApp::~CFCGIApp()
{
    delete CLog::getInstance();
    delete CSyncHelper::getInstance();
    delete CMessageParser::getInstance();
    delete CConfigHelper::getInstance();
    delete CTemplateHelper::getInstance();
	delete CTextHelper::getInstance();

    delete CManagers::getInstance();
}

void CFCGIApp::run()
{
	init();
	CManagers::getInstance()->lock();
	CConfigHelper* gs = CConfigHelper::getInstance();

	const int threadsCnt = gs->getIntParamValue("threadCnt", 2);
	std::thread* threads = new std::thread[threadsCnt];
	for(int i = 0; i < threadsCnt; i++)
	{
		threads[i] = std::thread(&CFCGIApp::requestHandler, this);
	}

	for(int i = 0; i < threadsCnt; i++)
	{
		threads[i].join();
	}
	delete [] threads;
}

void CFCGIApp::requestHandler()
{
	CFCGIRequestHandler fcgi(listenSocket);
	while(fcgi.acceptRequest())
	{
		CFCGIRequest newRequest(&fcgi);
		newRequest.response();
		fcgi.endRequest();
	}
}

void CFCGIApp::init()
{
	CLog::getInstance();
	CConfigHelper* gs = CConfigHelper::getInstance();

	srand((unsigned int)time(NULL));

	if(FCGX_Init())
	{
		CLog::getInstance()->addError("FCGX_Init fail");
	}
	std::string port = ":" + gs->getStringParamValue("port", "8000");
	listenSocket = FCGX_OpenSocket(port.c_str(), gs->getIntParamValue("listenQueueBacklog", 500));
    if(listenSocket < 0)
    {
		CLog::getInstance()->addError("Socket creation fail");
	}

	CManagers* managers = CManagers::getInstance();
	if(managers->getResourceManager()	== NULL)	managers->setResourceManager(new CResourceManager());
	if(managers->getSessionManager()	== NULL)	managers->setSessionManager(new CSessionManager());
	if(managers->getPageManager() 		== NULL) 	managers->setPageManager(new CPageManager());
	if(managers->getCommandManager()	== NULL)	managers->setCommandManager(new CDefaultCommandManager());
	if(managers->getDBManager()	 		== NULL)	managers->setDBManager(new CMySQLDBManager());
	
	if(gs->getBoolParamValue("isHasDB"))
	{
		std::string dbName = gs->getStringParamValue("dbName");
		std::string dbUser = gs->getStringParamValue("dbUser");
		std::string dbPass = gs->getStringParamValue("dbPass");
		std::string dbHost = gs->getStringParamValue("dbHost");
		managers->getDBManager()->initDB(dbName, dbUser, dbPass, dbHost);
	}
}
