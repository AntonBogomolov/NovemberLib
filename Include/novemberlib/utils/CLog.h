#ifndef CLOG_H
#define CLOG_H

#include "../helpers/CSyncHelper.h"
#include "../interfaces/ITemplateSingleton.h"

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

class CLog : public ITemplateSingleton<CLog>
{
	public:
		static const int LOG_PRINT_ALL 		= 2;
		static const int LOG_PRINT_WARNINGS	= 1;
		static const int LOG_PRINT_ERRORS	= 0;

		void setPrintLevel(const int printLevel);
		int  getPrintLevel() const;
		std::string getLogData() const;

		template<class T>
		void addInfo(const T infoMessage)
		{
			if (printLevel < LOG_PRINT_ALL) return;

			CSyncHelper::getInstance()->getLogMutex()->lock();
			converter << infoMessage;
			std::string message = converter.str();
			converter.str("");

			//if (printLevel >= LOG_PRINT_ALL)
			//{			
			std::string date = getDataAndTime();
			std::cout << date << message << std::endl;
			logData += date;
			logData += message;
			logData += "\r\n";
			if (logData.size() > (size_t)logBufferLen) flushLogFile();
			//}

			CSyncHelper::getInstance()->getLogMutex()->unlock();
		}
		template<class T>
		void addWarning(const T warningMessage)
		{
			if (printLevel < LOG_PRINT_WARNINGS)

			CSyncHelper::getInstance()->getLogMutex()->lock();
			converter << warningMessage;
			std::string message = converter.str();
			converter.str("");

			//if(printLevel >= LOG_PRINT_WARNINGS)
			//{
			std::string date = getDataAndTime();
			std::cout << date << message << std::endl;
			logData += date;
			logData += message;
			logData += "\r\n";
			if (logData.size() > (size_t)logBufferLen) flushLogFile();
			//}
			CSyncHelper::getInstance()->getLogMutex()->unlock();
		}
		template<class T>
		void addError(const T errorMessage)
		{
			if (printLevel < LOG_PRINT_ERRORS) return;

			CSyncHelper::getInstance()->getLogMutex()->lock();
			converter << errorMessage;
			std::string message = converter.str();
			converter.str("");

			//if(printLevel >= LOG_PRINT_ERRORS)
			//{
			std::string date = getDataAndTime();
			std::cout << date << message << std::endl;
			logData += date;
			logData += message;
			logData += "\r\n";
			if (logData.size() > (size_t)logBufferLen) flushLogFile();
			//}
			CSyncHelper::getInstance()->getLogMutex()->unlock();
		}

		virtual ~CLog();
	protected:
	private:		
		long logBufferLen;		
		int printLevel;
		std::string logData;
		std::ostringstream converter;
		std::ofstream logFile;

		const std::string getDataAndTime() const;
		void flushLogFile();

		friend CLog* ITemplateSingleton::getInstance();
		CLog();
};

#endif // CLOG_H
