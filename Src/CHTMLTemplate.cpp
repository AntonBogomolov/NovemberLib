#include "../Include/novemberlib/CHTMLTemplate.h"

#include "../Include/novemberlib/utils/utils.h"
#include "../Include/novemberlib/utils/CLog.h"

CHTMLTemplate::CHTMLTemplate(const std::string& templateName,const std::string& templateData)
{
	this->templateName = templateName;
	this->templateData = templateData;
}

CHTMLTemplate::~CHTMLTemplate()
{
	//dtor
}

const std::string CHTMLTemplate::getName() const
{
	return templateName;
}

const std::string CHTMLTemplate::getHTMLData() const
{
	return templateData;
}

const std::string CHTMLTemplate::getHTMLData(const std::vector<std::string> *paramNames, const std::vector<std::string> *paramValues) const
{
	if(paramNames->size() != paramValues->size()) return "template error!";

	std::string tmpTemlateData = templateData;
	for(unsigned int i = 0; i < paramNames->size(); i++)
	{
		tmpTemlateData = replaceAll(tmpTemlateData, (*paramNames)[i],(*paramValues)[i]);
	}
	return tmpTemlateData;
}

const std::string CHTMLTemplate::getHTMLData(const std::map<std::string, std::string> *params) const
{
	std::string tmpTemlateData = templateData;
	for(auto it = params->begin(); it != params->end(); ++it)
	{
		const std::string param = it->first;
		std::string value = it->second;
		if(param.substr(0,3) == "{is")
		{
			tmpTemlateData = processLogicParams(tmpTemlateData ,param, value == "true");
			continue;
		}

		tmpTemlateData = replaceAll(tmpTemlateData, param, value);
	}
	return tmpTemlateData;
}

std::string CHTMLTemplate::processLogicParams(std::string& tmplData, const std::string& logicParam, bool logicVal) const
{
	size_t pos = 0;
	size_t paramLen = logicParam.length();
	std::string tmpTemlateData = tmplData;

	while(pos != std::string::npos)
	{
		pos = tmpTemlateData.find(logicParam, pos);
		if(pos == std::string::npos) continue;

		tmpTemlateData = tmpTemlateData.replace(pos,paramLen,"");
		int mode = 0;
		int parCnt = 1;
		int leftParPos  = pos;
		int rightParPos = pos;

		if(tmpTemlateData[pos] != '{') continue;
		while(parCnt > 0)
		{
			pos++;
			if(pos >= tmpTemlateData.length()) break;

			char symbol = tmpTemlateData[pos];

			if(symbol == '{') parCnt++;
			if(symbol == '}') parCnt--;
			if(parCnt == 0)
			{
				rightParPos = pos;
				int len = rightParPos - leftParPos + 1;
				if(mode == 0)
				{
					if(logicVal )
					{
						std::string trueStr = tmpTemlateData.substr(leftParPos, len);
						trueStr[0] = ' ';
						trueStr[len - 1] = ' ';
						tmpTemlateData = tmpTemlateData.replace(leftParPos,len,trueStr);
						leftParPos = leftParPos + len;
						pos++;
						parCnt++;
					}
					if(!logicVal)
					{
						tmpTemlateData = tmpTemlateData.replace(leftParPos,len,"");
						pos = leftParPos;
						parCnt++;
					}
					mode = 1;
				}else
				if(mode == 1)
				{
					if(!logicVal)
					{
						std::string trueStr = tmpTemlateData.substr(leftParPos, len);
						trueStr[0] = ' ';
						trueStr[len - 1] = ' ';
						tmpTemlateData = tmpTemlateData.replace(leftParPos,len,trueStr);
					}
					if(logicVal)
					{
						tmpTemlateData = tmpTemlateData.replace(leftParPos,len,"");
					}
					break;
				}

			}
		}

	}
	return tmpTemlateData;
}
