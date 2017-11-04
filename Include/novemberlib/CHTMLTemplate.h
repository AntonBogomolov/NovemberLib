#ifndef CHTMLTEMPLATE_H
#define CHTMLTEMPLATE_H

#include <string>
#include <vector>
#include <map>

class CHTMLTemplate
{
	public:
		const std::string getName() const;
		const std::string getHTMLData() const;
		const std::string getHTMLData(const std::vector<std::string> *paramNames, const std::vector<std::string> *paramValues) const;
		const std::string getHTMLData(const std::map<std::string, std::string> *params) const;

		CHTMLTemplate(const std::string& templateName,const std::string& templateData);
		~CHTMLTemplate();
	protected:
	private:
		std::string processLogicParams(std::string& tmplData ,const std::string& logicParam, bool logicVal) const;

		std::string templateName;
		std::string templateData;
};

#endif // CHTMLTEMPLATE_H
