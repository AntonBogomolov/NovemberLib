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
<<<<<<< HEAD
			if (printLevel != LOG_PRINT_ALL) return;
=======
			if (printLevel < LOG_PRINT_ALL) return;
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311

			CSyncHelper::getInstance()->getLogMutex()->lock();
			converter << infoMessage;
			std::string message = converter.str();
			converter.str("");

			//if (printLevel >= LOG_PRINT_ALL)
<<<<<<< HEAD
			//{
=======
			//{			
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
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
<<<<<<< HEAD
			if (printLevel == LOG_PRINT_ERRORS) return;
=======
			if (printLevel < LOG_PRINT_WARNINGS)
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311

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
<<<<<<< HEAD
			//if (printLevel != LOG_PRINT_ERRORS) return;
=======
			if (printLevel < LOG_PRINT_ERRORS) return;
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311

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
<<<<<<< HEAD
	private:
		long logBufferLen;
=======
	private:		
		long logBufferLen;		
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
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
