#ifndef CMESSAGEPARSER_H
#define CMESSAGEPARSER_H

#include "../interfaces/ITemplateSingleton.h"

#include <string>
#include <vector>

class CMessageParser : public ITemplateSingleton<CMessageParser>
{
	public:
		const std::string parseMessage(const std::string& inputText) const;
		const std::string removeTags(const std::string& text) const;

		void addAllowedTag(const std::string& tag, const bool isPair = true);

		virtual ~CMessageParser();
	protected:
	private:
		std::vector<std::string> allowTags;

		friend CMessageParser* ITemplateSingleton::getInstance();
		CMessageParser();
};

#endif // CMESSAGEPARSER_H
