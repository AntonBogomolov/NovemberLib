#ifndef CHTMLTEMPLATEMANAGER_H
#define CHTMLTEMPLATEMANAGER_H

#include "../interfaces/ITemplateSingleton.h"

#include <string>
#include <vector>

class CHTMLTemplate;
class CPath;

class CTemplateHelper : public ITemplateSingleton<CTemplateHelper>
{
	public:
		const CHTMLTemplate* loadTemplate(const CPath* templateDir, const std::string& templateFileName, const std::string& templateName);
		const CHTMLTemplate* findTemplate(const std::string& templateName) const;

		virtual ~CTemplateHelper();
	protected:
		std::vector<CHTMLTemplate*> templates;
	private:
		friend CTemplateHelper* ITemplateSingleton::getInstance();
		CTemplateHelper();
};

#endif // CHTMLTEMPLATEMANAGER_H
