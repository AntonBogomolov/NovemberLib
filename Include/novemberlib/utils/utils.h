#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

<<<<<<< HEAD
#define _CRT_SECURE_NO_WARNINGS
=======
#define _CRT_SECURE_NO_WARNINGS 
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311

#include "config.h"

#include <vector>

#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include <climits>

#include <sys/types.h>
#include <sys/stat.h>

<<<<<<< HEAD
const std::string urlDecode(const std::string& str);
const std::string urlEncode(const std::string& str);

const std::string replaceAll(const std::string& text, const std::string& findWhat, const std::string& replaceTo);
const std::string htmlspecialchars(const std::string& data);
const std::string unixTimeToDate(const long unixTime, const std::string &format = "%F (%a) %T");
void split(std::vector<std::string>& resultVector, const std::string& inputString, const std::string& splitChars);
void fastSplit(std::vector<std::string>& resultVector, const std::string& inputString, const char splitChar);
bool isFileExists(const std::string& fname);

const std::string toLowerRU(const std::string& inputStr);
const std::vector<std::string> splitAndTrim(const std::string& inputStr, const std::string& splitStr = ",", const std::string& trimStr = " ");
const std::string trim(const std::string& inputStr, const std::string& trimChar = " ");

const std::string decompressString(const std::string& str);
const std::string compressString(const std::string& str, int compressionlevel = 9);

=======
std::string replaceAll(const std::string& text, const std::string& findWhat,const std::string& replaceTo);
const std::string htmlspecialchars(const std::string& data);
void split(std::vector<std::string>& resultVector, const std::string& inputString, const std::string& splitChar);
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
inline std::string deleteChars(const std::string& str, const std::string& chars)
{
	std::string r;
    for(size_t i = 0; i < str.size(); i++)
    {
        size_t j = 0;
        bool done = false;
        while(!done && j < chars.size())
        {
            if(str[i] == chars[j]) done = true;
            j++;
        }
        if(!done) r += str[i];
	}

    return r;
}

<<<<<<< HEAD
=======
bool isFileExists(const std::string fname);

std::string unixTimeToDate(const long unixTime, const std::string format = "%F (%a) %T");

>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
template<class T>
inline std::string valueToString(const T& valueToConvert)
{
	std::ostringstream converter;
	converter << valueToConvert;

	if(converter.good()) return converter.str();
	else return "CONVERT ERROR";
}

#endif // UTILS_H_INCLUDED
