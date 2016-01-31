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

		virtual ~CConfigHelper();
	protected:
		bool isInit;

		std::ifstream file;
		std::map<std::string, std::string> params;
		void parsePamarString(const std::string& paramString);
		virtual void readConfigFromFile();
	private:
		friend CConfigHelper* ITemplateSingleton::getInstance();
		CConfigHelper();
};

#endif // CSETTINGSMANAGER_H
