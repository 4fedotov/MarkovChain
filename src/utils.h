#ifndef _UTILS_
#define _UTILS_

#include <iostream>
#include <string>
#include <regex>
#include <algorithm>
#include <locale>
#include <sstream>

#include <unicode/unistr.h>
#include <unicode/ustream.h>

class Utils
{
public:
    static void toLowerEachLang(string &s)
    {
        stringstream ss;
        ss << icu::UnicodeString(s.c_str()).toLower();
        s = ss.str();
    }
    
    static string preprocess(string s)
    {
        s.erase(std::remove(s.begin(), s.end(), '\n'), s.end());
        s.erase(std::remove_if(s.begin(), s.end(), ::ispunct), s.end());
        //transform(s.begin(), s.end(), s.begin(), ::tolower);
        toLowerEachLang(s);
        
        return s;
    }
};


#endif //_UTILS_