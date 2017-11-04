#include "../../Include/novemberlib/utils/CLog.h"

#include "../../Include/novemberlib/helpers/CConfigHelper.h"
#include "../../Include/novemberlib/managers/CManagers.h"
#include "../../Include/novemberlib/utils/utils.h"

#include <iostream>
#include <time.h>

CLog::CLog()
{
	CConfigHelper* gs = CConfigHelper::getInstance();
	std::string logFileName = "log.txt";
	int logPrintLevel = CLog::LOG_PRINT_ALL;
	logBufferLen = 10240;
<<<<<<< HEAD
	if(gs != NULL)
=======
	if(gs != NULL)	
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
	{
		logPrintLevel = gs->getIntParamValue("logLevel", 2);
		logBufferLen  = gs->getLongParamValue("logBufferLen", 10240);
		logFileName   = gs->getStringParamValue("logFileName", "log.txt");
	}
	logFile.open(logFileName, std::fstream::out | std::fstream::app);
	printLevel = logPrintLevel;
}

CLog::~CLog()
{
	flushLogFile();
	logFile.close();
}

void CLog::flushLogFile()
{
	if (!logFile.is_open()) return;

	logFile << logData << '\r' << '\n';
	logData = "";
}

void CLog::setPrintLevel(const int printLevel)
{
	this->printLevel = printLevel;
}

int  CLog::getPrintLevel() const
{
	return printLevel;
}

std::string CLog::getLogData() const
{
	return logData;
}

const std::string CLog::getDataAndTime() const
{
<<<<<<< HEAD
	std::string resultString;
=======
	std::string resultString = "";
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311

	time_t currUnixTime;
	time(&currUnixTime);

	resultString = "[ " + unixTimeToDate((long)currUnixTime) + " ] ";
	return resultString;
}
