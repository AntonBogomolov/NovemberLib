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
	if(gs != NULL)
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
	std::string resultString;

	time_t currUnixTime;
	time(&currUnixTime);

	resultString = "[ " + unixTimeToDate((long)currUnixTime) + " ] ";
	return resultString;
}
