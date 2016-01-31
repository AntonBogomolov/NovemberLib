#include "../../Include/novemberlib/helpers/CConfigHelper.h"
#include "../../Include/novemberlib/utils/CLog.h"

CConfigHelper::CConfigHelper()
{
	isInit = false;
}

CConfigHelper::~CConfigHelper()
{
	//dtor
}

void CConfigHelper::parsePamarString(const std::string& paramString)
{
	std::string buff = "";
	std::string varName  = "";
	std::string varValue = "";

	size_t commentPos = paramString.find("//");
	size_t delimPos   = paramString.find_first_of('=');
	if(delimPos == std::string::npos || delimPos == 0 || delimPos == paramString.length() - 1)
	{
		//CLog::getInstance()->addError("Cant parse this line: " + paramString + " in config.conf");
		return;
	}

	bool isVarName = true;
	size_t lineEnd = paramString.length();
	if(commentPos != std::string::npos) lineEnd = commentPos;
	for(size_t pos = 0; pos < lineEnd; ++pos )
	{
		char currChar = paramString[pos];
		if(currChar < '.') continue;
		if(currChar > '9' && currChar < '=') continue;
		if(currChar > '=' && currChar < 'A') continue;
		if(currChar > '_' && currChar < 'a') continue;
		if(currChar > 'z') continue;

		if(currChar == '=')
		{
			if(isVarName)
			{
				varName = buff;
				buff = "";
				isVarName = false;
			}
			else
			{
				//CLog::getInstance()->addError("Cant parse this line: " + paramString + " in config.conf");
				return;
			}
		}
		else
		{
			buff += currChar;
		}
	}
	varValue = buff;

	if(varName.length() > (size_t)0 && varValue.length() > (size_t)0)
	{
		params[varName] = varValue;
	}
	else
	{
		//CLog::getInstance()->addError("Cant parse this line: " + paramString + " in config.conf");
		return;
	}
}

void CConfigHelper::readConfigFromFile()
{
	std::string tmpStr = "";

	try
	{
		file.open("config.conf");
		if(!file.is_open())
		{
			CLog::getInstance()->addError("Cant open config.conf");
			return;
		}

		while(!file.eof())
		{
			std::getline(file, tmpStr);
			parsePamarString(tmpStr);
		}
	}
	catch(...)
	{
		CLog::getInstance()->addError("Cant read config.conf");
	}

	isInit = true;
	file.close();
}

bool CConfigHelper::getIsInit() const
{
	return isInit;
}

int  CConfigHelper::getIntParamValue(const std::string& paramName, const int defValue)
{
	if(!getIsInit())readConfigFromFile();

	auto it = params.find(paramName);
	if (it == params.end()) return defValue;

	int value = defValue;
	try
	{
		value = std::stoi(it->second);
	}
	catch(...)
	{
		CLog::getInstance()->addWarning("Cant get int param: " + it->first + " " + it->second);
		value = defValue;
	}

	return value;
}

long CConfigHelper::getLongParamValue(const std::string& paramName, const long defValue)
{
	if(!getIsInit())readConfigFromFile();

	auto it = params.find(paramName);
	if (it == params.end()) return defValue;

	long value = defValue;
	try
	{
		value = std::stol(it->second);
	}
	catch(...)
	{
		CLog::getInstance()->addWarning("Cant get long param: " + it->first + " " + it->second);
		value = defValue;
	}

	return value;
}

bool CConfigHelper::getBoolParamValue(const std::string& paramName, const bool defValue)
{
	if(!getIsInit())readConfigFromFile();

	auto it = params.find(paramName);
	if (it == params.end()) return defValue;

	bool value = it->second == "true";
	return value;
}

const std::string CConfigHelper::getStringParamValue(const std::string& paramName, const std::string& defValue)
{
	if(!getIsInit())readConfigFromFile();

	auto it = params.find(paramName);
	if (it == params.end()) return defValue;

	return it->second;
}
