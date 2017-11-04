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

<<<<<<< HEAD
void CConfigHelper::parseParamString(const std::string& paramString)
=======
void CConfigHelper::parsePamarString(const std::string& paramString)
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
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
<<<<<<< HEAD

		if(currChar < '#') continue;
        if(currChar >  'z') continue;
        if(currChar == ';') continue;
		if(isVarName)
		{
            if(currChar < '.') continue;
            if(currChar > '9' && currChar < '=') continue;
            if(currChar > '=' && currChar < 'A') continue;
            if(currChar > '_' && currChar < 'a') continue;
            //if(currChar > 'z') continue;
		}
=======
		if(currChar < '.') continue;
		if(currChar > '9' && currChar < '=') continue;
		if(currChar > '=' && currChar < 'A') continue;
		if(currChar > '_' && currChar < 'a') continue;
		if(currChar > 'z') continue;
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311

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
<<<<<<< HEAD
				//return;
				buff += currChar;
				continue;
=======
				return;
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
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
<<<<<<< HEAD
			parseParamString(tmpStr);
=======
			parsePamarString(tmpStr);
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
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

<<<<<<< HEAD
void CConfigHelper::addParamToUpdateQuery(const std::string& paramName, const std::string& newValue)
{
    paramsToUpdate[paramName] = newValue;
}

void CConfigHelper::updateParams()
{
    if(paramsToUpdate.empty()) return;

    std::string tmpStr;
    std::string resultData = "";

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
			bool isFindedParam = false;
			for(auto it = paramsToUpdate.begin(); it != paramsToUpdate.end(); ++it)
			{
                std::string currParamName = it->first;
                if(tmpStr.find(currParamName) != std::string::npos)
                {
                    resultData += currParamName;
                    resultData += " = ";
                    resultData += it->second;
                    resultData += ";";
                    resultData += '\n';
                    isFindedParam = true;
                    paramsToUpdate.erase(it);
                    break;
                }
			}
            if(!isFindedParam)
            {
                resultData += tmpStr;
                resultData += '\n';
            }
		}
	}
	catch(...)
	{
		CLog::getInstance()->addError("Cant read config.conf");
	}
	file.close();

	for(auto it = paramsToUpdate.begin(); it != paramsToUpdate.end(); ++it)
    {
        resultData += it->first;
        resultData += " = ";
        resultData += it->second;
        resultData += ";";
        resultData += '\n';
    }

	std::ofstream updateFile;
	try
	{
        updateFile.open("config.conf");
        updateFile << resultData;
	}
	catch(...)
	{
		CLog::getInstance()->addError("Cant update config.conf");
	}
	updateFile.close();
}

int CConfigHelper::getIntParamValue(const std::string& paramName, const int defValue)
=======
int  CConfigHelper::getIntParamValue(const std::string& paramName, const int defValue)
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
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
