#pragma once

#include "util/data/list.hpp"


namespace Base
{
    /* Escapes quotations in the given string. */
    EXPORT string stringEscapeQuotes(const string& str);

    /* Converts a wide string to a UTF-8 encoded string and vice versa. */
    EXPORT string wstringToString(const wstring& str);
    EXPORT wstring stringToWstring(const string& wstr);

    /* Creates a list of all the substrings in str separated by sep. */
    EXPORT List<string> stringSplit(const string& str, const string& sep);
    
    EXPORT string stringSubstring(const string& str, int from, int length);
    EXPORT string stringErase(const string& str, int from, int length);
    EXPORT string stringInsert(const string& str, const string& substr, int index);
    EXPORT string stringRepeat(const string& str, int count);

    /* Replaces all occurrences of the given substring and returns the modified string. */
    EXPORT string stringReplace(const string& str, const string& from, const string& to);

    /* Returns the amount of occurrences of a substring. */
    EXPORT int stringGetCount(const string& str, const string& sub, int index = 0);

    /* Converts a value into a string. */
    template<typename T> string toString(const T& val)
    {
        std::stringstream ss;
        ss << val;
        return ss.str();
    }
    
    EXPORT List<string> stringGetLines(const string& str);

    /* Converts a value into a string with a given amount of decimal places. */
    template<typename T> string toStringPrec(const T& val, int prec)
    {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(prec) << val;
        return ss.str();
    }
}