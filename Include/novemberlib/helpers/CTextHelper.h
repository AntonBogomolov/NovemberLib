#ifndef CTEXTHELPER_H
#define CTEXTHELPER_H

#include "../interfaces/ITemplateSingleton.h"

#include <string>
#include <vector>
#include <map>

class CTextHelper : public ITemplateSingleton<CTextHelper>
{
	public:		
		const std::string getText(const std::string& textID, const std::string& lang);
		const std::string getText(const std::string& textID);
		const std::string& getLanguage() const;

		virtual ~CTextHelper();
	protected:
		std::map<std::string, std::map<std::string, std::string> > textMap;
		std::string currLang;
	private:
		void parseText(const std::string& line);
		void loadFromFile(const std::string& fileName);
		void addText(const std::string& textID, const std::string& text, const std::string& lang = "en_US");
		void setLanguage(const std::string& lang);

		friend CTextHelper* ITemplateSingleton::getInstance();
		CTextHelper();
};

#endif // CTEXTHELPER_H
