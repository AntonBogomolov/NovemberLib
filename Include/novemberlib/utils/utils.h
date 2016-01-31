#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#define _CRT_SECURE_NO_WARNINGS 

#include "config.h"

#include <vector>

#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include <climits>

#include <sys/types.h>
#include <sys/stat.h>

std::string replaceAll(const std::string& text, const std::string& findWhat,const std::string& replaceTo);
const std::string htmlspecialchars(const std::string& data);
void split(std::vector<std::string>& resultVector, const std::string& inputString, const std::string& splitChar);
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

bool isFileExists(const std::string fname);

std::string unixTimeToDate(const long unixTime, const std::string format = "%F (%a) %T");

template<class T>
inline std::string valueToString(const T& valueToConvert)
{
	std::ostringstream converter;
	converter << valueToConvert;

	if(converter.good()) return converter.str();
	else return "CONVERT ERROR";
}

#endif // UTILS_H_INCLUDED
