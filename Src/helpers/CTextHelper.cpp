#include "../../Include/novemberlib/helpers/CTextHelper.h"

#include "../../Include/novemberlib/utils/CLog.h"
#include "../../Include/novemberlib/helpers/CConfigHelper.h"
#include <fstream>

CTextHelper::CTextHelper()
{
	currLang = "en_US";
	CConfigHelper* gs = CConfigHelper::getInstance();	
	if (gs != NULL)
	{
		currLang = gs->getStringParamValue("lang", "en_US");
		std::string fileName = gs->getStringParamValue("textConstantsFileName", "");
		if(fileName.size() > (size_t)0)loadFromFile(fileName);
	}	
}

CTextHelper::~CTextHelper()
{
	//dtor
}

void CTextHelper::addText(const std::string& textID, const std::string& text, const std::string& lang)
{
	std::map<std::string, std::string> langMap;

	if (textMap.find(textID) == textMap.end())
	{
		langMap[lang] = text;
		textMap[textID] = langMap;
	}
	else
	{
		textMap[textID].insert(std::make_pair(lang, text));
	}	
}

const std::string CTextHelper::getText(const std::string& textID, const std::string& lang)
{
	std::map<std::string, std::string> langMap;
	if (textMap.find(textID) == textMap.end()) return "Text with ID: " + textID + " not found, sorry";
	langMap = textMap[textID];
	for (std::map<std::string, std::string>::iterator it = langMap.begin(); it != langMap.end(); ++it)
	{
		std::string storedLang = it->first;
		if (storedLang.find(lang) != std::string::npos) return it->second;
	}
	return "Text with ID: " + textID + " and language: " + lang + " not found, sorry";	
}

const std::string CTextHelper::getText(const std::string& textID)
{
	return getText(textID, currLang);
}

void CTextHelper::loadFromFile(const std::string& fileName)
{
	std::string tmpStr = "";
	std::ifstream file;
	try
	{
		file.open(fileName);
		if (!file.is_open())
		{
			CLog::getInstance()->addError("Cant open" + fileName);
			return;
		}

		while (!file.eof())
		{
			std::getline(file, tmpStr);
			parseText(tmpStr);
		}
	}
	catch (...)
	{
		CLog::getInstance()->addError("Cant read " + fileName);
	}
		
	file.close();
}

void CTextHelper::parseText(const std::string& line)
{
	std::string buff = "";
	std::string varName = "";
	std::string varValue = "";
	std::string lang = currLang;

	size_t commentPos = line.find("//");
	size_t delimPos = line.find_first_of('=');
	if (delimPos == std::string::npos || delimPos == 0 || delimPos == line.length() - 1)
	{		
		return;
	}

	bool isVarName  = true;
	bool isLangName = false;
	size_t lineEnd = line.length();
	if (commentPos != std::string::npos) lineEnd = commentPos;
	for (size_t pos = 0; pos < lineEnd; ++pos)
	{
		char currChar = line[pos];
		if (currChar < ' ') continue;
		
		if(currChar == '=')
		{
			if (isLangName)
			{
				lang = buff;
				buff = "";
				isVarName = false;
				isLangName = false;
				varValue = line.substr(pos + 1);
				pos = lineEnd;
			}
			if (isVarName)
			{
				varName = buff;
				buff = "";
				isVarName  = false;
				isLangName = false;
				varValue = line.substr(pos + 1);
				pos = lineEnd;
			}
		
		}
		else
		if(currChar == '|')
		{
			if (isVarName)
			{
				varName = buff;
				buff = "";
				isVarName  = false;
				isLangName = true;
				continue;
			}			
		}
		else
		{
			if ((isVarName || isLangName) && currChar == ' ') continue;
			buff += currChar;
		}
	}
	
	if (varName.length() > (size_t)0 && varValue.length() > (size_t)0)
	{
		addText(varName, varValue, lang);
	}
	else
	{		
		return;
	}
}

void CTextHelper::setLanguage(const std::string& lang)
{
	currLang = lang;
}

const std::string& CTextHelper::getLanguage() const
{
	return currLang;
}