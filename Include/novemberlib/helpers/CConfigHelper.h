#ifndef CSETTINGSMANAGER_H
#define CSETTINGSMANAGER_H

#include "../interfaces/ITemplateSingleton.h"

#include <map>
#include <string>
#include <fstream>

class CConfigHelper : public ITemplateSingleton<CConfigHelper>
{
	public:
		bool getIsInit() const;

		int  getIntParamValue(const std::string& paramName, const int defValue = 0);
		long getLongParamValue(const std::string& paramName, const long defValue = 0l);
		bool getBoolParamValue(const std::string& paramName, const bool defValue = false);
		const std::string getStringParamValue(const std::string& paramName, const std::string& defValue = "");

<<<<<<< HEAD
		void addParamToUpdateQuery(const std::string& paramName, const std::string& newValue);
		void updateParams();

=======
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
		virtual ~CConfigHelper();
	protected:
		bool isInit;

		std::ifstream file;
		std::map<std::string, std::string> params;
<<<<<<< HEAD
		std::map<std::string, std::string> paramsToUpdate;
		void parseParamString(const std::string& paramString);
=======
		void parsePamarString(const std::string& paramString);
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
		virtual void readConfigFromFile();
	private:
		friend CConfigHelper* ITemplateSingleton::getInstance();
		CConfigHelper();
};

#endif // CSETTINGSMANAGER_H
