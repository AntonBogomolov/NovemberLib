#include "../../Include/novemberlib/helpers/CMessageParser.h"

#include "../../Include/novemberlib/utils/CLog.h"
#include "../../Include/novemberlib/utils/utils.h"

CMessageParser::CMessageParser()
{
//	allowTags.push_back("<p>");
//	allowTags.push_back("</p>");
//	allowTags.push_back("<b>");
//	allowTags.push_back("</b>");
//	allowTags.push_back("<i>");
//	allowTags.push_back("</i>");
//	allowTags.push_back("<u>");
//	allowTags.push_back("</u>");
//	allowTags.push_back("<li>");
//	allowTags.push_back("</li>");
//	allowTags.push_back("<ol>");
//	allowTags.push_back("</ol>");
//	allowTags.push_back("<ul>");
//	allowTags.push_back("</ul>");
//	allowTags.push_back("<br>");
//	allowTags.push_back("<h1>");
//	allowTags.push_back("</h1>");
//	allowTags.push_back("<h2>");
//	allowTags.push_back("</h2>");
}

CMessageParser::~CMessageParser()
{
	//dtor
}

void CMessageParser::addAllowedTag(const std::string& tag, const bool isPair)
{
	std::string allowTag = "<"  + tag + ">";
	std::string closeTag = "</" + tag + ">";

	allowTags.push_back(allowTag);
	if(isPair) allowTags.push_back(closeTag);
}

const std::string CMessageParser::removeTags(const std::string& text) const
{
	std::string result = text;

	for(unsigned int i = 0; i < text.length(); i++)
	{
		if(text[i] == '<') result[i] = '[';
		else
		if(text[i] == '>') result[i] = ']';
	}

	std::string allowTag;
	std::string currTag;
	for(unsigned int i = 0; i < allowTags.size(); i++)
	{
		allowTag = allowTags[i];
		currTag = allowTag;
		currTag[0] = '[';
		currTag[currTag.length() - 1] = ']';
 		result = replaceAll(result, currTag, allowTag);
	}

	return result;
}

const std::string CMessageParser::parseMessage(const std::string& inputText) const
{
//	std::string message = "";
//    message.reserve((int)(inputText.size() * 1.5f));
//
//    std::string text = removeTags(inputText);
//
//	const int ST_INIT 	  = 0;
//	const int ST_ITAL_BEG = 1;
//	const int ST_BOLD_BEG = 3;
//	const int ST_SPOILER_BEG = 5;
//	const int ST_LINK_BEG = 9;
//
//	std::vector<int> states;
//	std::vector<int> startPos;
//	states.push_back(ST_INIT);
//	startPos.push_back(0);
//
//	for(int pos = 0; pos < (int)text.size(); ++pos)
//    {
//		int state = states.back();
//		int stPos = startPos.back();
//        switch(text[pos])
//        {
//			case '_':
//				if(state == ST_BOLD_BEG)
//				{
//					message.insert(stPos, "<b>");
//					message += "</b>";
//					states.pop_back();
//					startPos.pop_back();
//					continue;
//				}
//				else
//				{
//					states.push_back(ST_BOLD_BEG);
//					startPos.push_back(message.length());
//					continue;
//				}
//
//			break;
//			case '*':
//				if(state == ST_ITAL_BEG)
//				{
//					message.insert(stPos, "<i>");
//					message += "</i>";
//					states.pop_back();
//					startPos.pop_back();
//					continue;
//				}
//				else
//				{
//					states.push_back(ST_ITAL_BEG);
//					startPos.push_back(message.length());
//					continue;
//				}
//			break;
//
//			case '%':
//				if(state == ST_SPOILER_BEG)
//				{
//					message.insert(stPos, "<span class='spoiler'>");
//					message += "</span>";
//					pos++;
//					states.pop_back();
//					startPos.pop_back();
//				}
//				else
//				{
//					if((int)text.length() - 1 > pos && text[pos + 1] == '%')
//					{
//						states.push_back(ST_SPOILER_BEG);
//						startPos.push_back(message.length());
//						pos++;
//					}
//					else
//					{
//						message += "%";
//					}
//					continue;
//				}
//			break;
//
//			case '-':
//				if((int)text.length() - 1 > pos && text[pos + 1] == '-')
//				{
//					states.push_back(ST_LINK_BEG);
//					startPos.push_back(message.length());
//					pos++;
//				}
//				else
//				{
//					message += "-";
//				}
//			break;
//
//			case ' ':
//				if(state == ST_LINK_BEG)
//				{
//					message.insert(stPos, "<span class='message_link'>");
//					message += "</span>";
//					states.pop_back();
//					startPos.pop_back();
//				}
//				message += " ";
//			break;
//			default:
//				message += text[pos];
//			break;
//        }
//	}

    return removeTags(inputText);
}
