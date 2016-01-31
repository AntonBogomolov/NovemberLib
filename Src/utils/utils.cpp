#include "../../Include/novemberlib/utils/utils.h"

#include <fstream>
#include <algorithm>
#include <sys/stat.h>

// VS2015 FIX
#ifdef VS2015
extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }
#endif // VS2015


std::string unixTimeToDate(const long unixTime, const std::string format)
{
	if(unixTime == 0) return "";

	time_t time = unixTime;
	struct tm *tm = localtime(&time);
	char date[30];
	strftime(date, sizeof(date), format.c_str(), tm);

	std::string dateStr = date;
	return dateStr;
}

std::string replaceAll(const std::string& text, const std::string& findWhat,const std::string& replaceTo)
{
    std::string result = text;
    for(unsigned int index = 0; index = result.find(findWhat, index), index != std::string::npos;)
    {
        result.replace(index, findWhat.length(), replaceTo);
        index += replaceTo.length();
    }
    return result;
}

void split(std::vector<std::string>& resultVector, const std::string& inputString, const std::string& splitChar)
{
    resultVector.clear();

    std::string tmpStr = "";
    for(unsigned int i = 0; i < inputString.size(); i++)
    {
        if(inputString[i] == splitChar[0])
        {
            if((int)tmpStr.size() > 0) resultVector.push_back(tmpStr);
            tmpStr = "";
        }
        else
        {
            tmpStr += inputString[i];
        }
    }

    if((int)tmpStr.size() > 0) resultVector.push_back(tmpStr);
}

bool isFileExists(const std::string fname)
{
	struct stat buffer;
	return (stat(fname.c_str(), &buffer) == 0);
}

const std::string htmlspecialchars(const std::string& data)
{
    std::string buffer;
    buffer.reserve(data.size());
    for(size_t pos = 0; pos != data.size(); ++pos)
    {
        switch(data[pos])
        {
        case '&':
            buffer.append("&amp;");
            break;
        case '\"':
            buffer.append("&quot;");
            break;
        case '\'':
            buffer.append("&apos;");
            break;
        case '<':
            buffer.append("&lt;");
            break;
        case '>':
            buffer.append("&gt;");
            break;
        default:
            buffer.append(&data[pos], 1);
            break;
        }
    }
    return buffer;
}
