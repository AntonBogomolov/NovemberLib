#include "../../Include/novemberlib/helpers/CTemplateHelper.h"

#include "../../Include/novemberlib/CHTMLTemplate.h"
#include "../../Include/novemberlib/managers/CManagers.h"
#include "../../Include/novemberlib/managers/CResourceManager.h"
#include "../../Include/novemberlib/helpers/CPathsHelper.h"

CTemplateHelper::~CTemplateHelper()
{
    for(auto it = templates.begin(); it != templates.end(); ++it)
    {
        CHTMLTemplate* currTemplate = (*it);
        delete currTemplate;
    }
}

CTemplateHelper::CTemplateHelper()
{

}

const CHTMLTemplate* CTemplateHelper::loadTemplate(const CPath* templateDir, const std::string& templateFileName, const std::string& templateName)
{
<<<<<<< HEAD
    std::string templateContent;
=======
    std::string templateContent = "";
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
    std::string contentType = templateDir->getIsOnlyForSiteContent() ? CPathsHelper::getInstance()->getSiteContentDir() : CPathsHelper::getInstance()->getUserContentDir();
    std::string templatePath = contentType + templateDir->getRelativeDir() + templateFileName;

    templateContent = CManagers::getInstance()->getResourceManager()->loadTextFile(templatePath);
    CHTMLTemplate* newHTMLTemplate = new CHTMLTemplate(templateName, templateContent);
    templates.push_back(newHTMLTemplate);

    return newHTMLTemplate;
}

const CHTMLTemplate* CTemplateHelper::findTemplate(const std::string& templateName) const
{
    for(auto it = templates.begin(); it != templates.end(); ++it)
    {
        CHTMLTemplate* currTemplate = (*it);
        if(currTemplate->getName().compare(templateName) == 0)
        {
            return currTemplate;
        }
    }
    return NULL;
}
