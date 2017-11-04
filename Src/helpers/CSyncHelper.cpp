#include "../../Include/novemberlib/helpers/CSyncHelper.h"

CSyncHelper::CSyncHelper()
{
	mtxResource = new std::mutex();
	mtxLog 		= new std::mutex();
	mtxFile		= new std::mutex();
	mtxDB		= new std::mutex();
	mtxRequest	= new std::mutex();
	mtxPage 	= new std::mutex();
}

CSyncHelper::~CSyncHelper()
{
	delete mtxLog;
	delete mtxFile;
	delete mtxResource;
	delete mtxDB;
	delete mtxRequest;
	delete mtxPage;
}

std::mutex* CSyncHelper::getLogMutex() const
{
	return mtxLog;
}

std::mutex* CSyncHelper::getResourceMutex() const
{
	return mtxResource;
}

std::mutex* CSyncHelper::getFileMutex() const
{
	return mtxFile;
}

std::mutex* CSyncHelper::getDBMutex() const
{
	return mtxDB;
}

std::mutex* CSyncHelper::getRequestMutex() const
{
	return mtxRequest;
}

std::mutex* CSyncHelper::getPageMutex() const
{
	return mtxPage;
}
