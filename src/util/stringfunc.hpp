#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <wchar.h>
#include <stdlib.h>
#include <iomanip>

using std::string;
using std::wstring;

namespace Base
{
    enum FontStyle
    {
        NORMAL,
        BOLD
    };
    
    /* Escapes quotations in the given string. */
    string stringEscapeQuotes(string str);

    /* Converts a wide string to a UTF-8 encoded string and vice versa. */
    string wstringToString(wstring str);
    wstring stringToWstring(string wstr);

    /* Creates a list of all the substrings in str separated by sep. */
    std::vector<string> stringSplit(string str, string sep);
    
    string stringSubstring(string str, int from, int length);
    string stringErase(string str, int from, int length);
    string stringInsert(string str, string substr, int index);
    string stringRepeat(string str, int count);

    /* Replaces all occurrences of the given substring and returns the modified string. */
    string stringReplace(string str, string from, string to);

    /* Returns the amount of occurrences of a substring. */
    int stringGetCount(string str, string sub, int index = 0);

    /* Converts a value into a string. */
    template<typename T> inline string toString(T val)
    {
        std::stringstream ss;
        ss << val;
        return ss.str();
    }    

    /* Converts a value into a string with a given amount of decimal places. */
    template<typename T> inline string toStringPrec(T val, int prec)
    {
        std::stringstream ss;
        ss << std::setprecision(prec) << val;
        return ss.str();
    }    
}