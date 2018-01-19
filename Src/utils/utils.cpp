#include "../../Include/novemberlib/utils/utils.h"

#include <fstream>
#include <algorithm>
#include <sys/stat.h>
#include <iostream>
#include <string>

#include <zlib.h>

// VS2015 FIX
#ifdef VS2015
extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }
#endif // VS2015

const std::string toLowerRU(const std::string& inputStr)
{
    char chH;
    char chL;

    std::string outputStr = "";
    for(unsigned int i = 0; i < inputStr.length(); i++)
    {
        chH = inputStr[i];
        if(chH >= 'A' && chH <= 'Z') outputStr += chH + ('a' - 'A');
        else
        if(chH == -48)
        {
            chL = inputStr[i+1];
            if(chL >= -112 && chL <= -81)
            {
                if(chL + 32 > -65 )
                {
                    outputStr += -47;
                    outputStr += chL + 32 - 64;
                }
                else
                {
                    outputStr += -48;
                    outputStr += chL + 32;
                }
            }
            else
            {
                outputStr += chH;
                outputStr += chL;
            }
            i++;
        }
        else
        outputStr += chH;
    }
    return outputStr;
}

const std::vector<std::string> splitAndTrim(const std::string& inputStr, const std::string& splitStr, const std::string& trimStr)
{
    std::vector<std::string> result;
    std::vector<std::string> splitResult;
    split(splitResult, inputStr, splitStr);
    for(auto it = splitResult.begin(); it != splitResult.end(); ++it)
    {
        std::string currStr = (*it);
        if(currStr.length() == 0) continue;
        std::string trimResult = trim(currStr, trimStr);
        result.push_back(trimResult);
    }
    return result;
}

const std::string trim(const std::string& inputStr, const std::string& trimChar)
{
    std::string result = "";
    if(inputStr.length() == 0 || trimChar.length() == 0) return result;

    result = inputStr;
    for(unsigned int i = 0; i < inputStr.length(); i++)
    {
        if(trimChar.find_first_of(inputStr[i]) == std::string::npos)
        {
            result = inputStr.substr(i);
            break;
        }
    }
    for(unsigned int i = inputStr.length() - 1; i > 0; i--)
    {
        if(trimChar.find_first_of(inputStr[i]) == std::string::npos)
        {
            result = result.substr(0, i+1);
            break;
        }
    }
    return result;
}

const std::string unixTimeToDate(const long unixTime, const std::string& format)
{
	if(unixTime == 0) return "";

	time_t time = unixTime;
	struct tm *tm = localtime(&time);
	char date[30];
	strftime(date, sizeof(date), format.c_str(), tm);

	std::string dateStr = date;
	return dateStr;
}

const std::string replaceAll(const std::string& text, const std::string& findWhat,const std::string& replaceTo)
{
    std::string result = text;
    for(int index = 0; index = (int)result.find(findWhat, index), index != (int)std::string::npos;)
    {
        result.replace(index, findWhat.length(), replaceTo);
        index += replaceTo.length();
    }
    return result;
}

void split(std::vector<std::string>& resultVector, const std::string& inputString, const std::string& splitChars)
{
    resultVector.clear();

    std::string tmpStr = "";
    for(unsigned int i = 0; i < inputString.size(); i++)
    {
        //if(inputString[i] == splitChar[0])
        if(splitChars.find_first_of(inputString[i]) != std::string::npos)
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
void fastSplit(std::vector<std::string>& resultVector, const std::string& inputString, const char splitChar)
{
    resultVector.clear();

    std::string tmpStr = "";
    for(unsigned int i = 0; i < inputString.size(); i++)
    {
        if(inputString[i] == splitChar)
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

bool isFileExists(const std::string& fname)
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

const std::string urlEncode(const std::string& str)
{
    static const char lookup[]= "0123456789abcdef";
    std::string e="";
    for(int i=0, ix=str.length(); i<ix; i++)
    {
        const char& c = str[i];
        if ( (48 <= c && c <= 57) ||//0-9
             (65 <= c && c <= 90) ||//abc...xyz
             (97 <= c && c <= 122) || //ABC...XYZ
             (c=='-' || c=='_' || c=='.' || c=='~')
        )
        {
            e += c;
        }
        else
        {
            e += '%';
            e += lookup[ (c&0xF0)>>4 ];
            e += lookup[ (c&0x0F) ];
        }
    }
    return e;
}

const std::string urlDecode(const std::string& str)
{
    std::string ret;
    char ch;
    unsigned int i, ii, len = str.length();

    for (i=0; i < len; i++){
        if(str[i] != '%'){
            if(str[i] == '+')
                ret += ' ';
            else
                ret += str[i];
        }else{
            sscanf(str.substr(i + 1, 2).c_str(), "%4x", &ii);
            ch = static_cast<char>(ii);
            ret += ch;
            i = i + 2;
        }
    }
    return ret;
}

/** Compress a STL string using zlib with given compression level and return
  * the binary data. */
const std::string compressString(const std::string& str, int compressionlevel)
{
    z_stream zs;                        // z_stream is zlib's control structure
    //memset(&zs, 0, sizeof(zs));

    if (deflateInit(&zs, compressionlevel) != Z_OK)
        throw(std::runtime_error("deflateInit failed while compressing."));

    zs.next_in = (Bytef*)str.data();
    zs.avail_in = str.size();           // set the z_stream's input

    int ret;
    char outbuffer[32768];
    std::string outstring;

    // retrieve the compressed bytes blockwise
    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = deflate(&zs, Z_FINISH);

        if (outstring.size() < zs.total_out) {
            // append the block to the output string
            outstring.append(outbuffer,
                             zs.total_out - outstring.size());
        }
    } while (ret == Z_OK);

    deflateEnd(&zs);

    if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
        std::ostringstream oss;
        oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
        throw(std::runtime_error(oss.str()));
    }

    return outstring;
}

/** Decompress an STL string using zlib and return the original data. */
const std::string decompressString(const std::string& str)
{
    z_stream zs;                        // z_stream is zlib's control structure
    //memset(&zs, 0, sizeof(zs));

    if (inflateInit(&zs) != Z_OK)
        throw(std::runtime_error("inflateInit failed while decompressing."));

    zs.next_in = (Bytef*)str.data();
    zs.avail_in = str.size();

    int ret;
    char outbuffer[32768];
    std::string outstring;

    // get the decompressed bytes blockwise using repeated calls to inflate
    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = inflate(&zs, 0);

        if (outstring.size() < zs.total_out) {
            outstring.append(outbuffer,
                             zs.total_out - outstring.size());
        }

    } while (ret == Z_OK);

    inflateEnd(&zs);

    if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
        std::ostringstream oss;
        oss << "Exception during zlib decompression: (" << ret << ") "
            << zs.msg;
        throw(std::runtime_error(oss.str()));
    }

    return outstring;
}
