#pragma once

#include "util/data/list.hpp"

namespace Base
{
    /* Escapes quotations in the given string. */
    EXPORT string stringEscapeQuotes(string str);

    /* Converts a wide string to a UTF-8 encoded string and vice versa. */
    EXPORT string wstringToString(wstring str);
    EXPORT wstring stringToWstring(string wstr);

    /* Creates a list of all the substrings in str separated by sep. */
    EXPORT List<string> stringSplit(string str, string sep);
    
    EXPORT string stringSubstring(string str, int from, int length);
    EXPORT string stringErase(string str, int from, int length);
    EXPORT string stringInsert(string str, string substr, int index);
    EXPORT string stringRepeat(string str, int count);

    /* Replaces all occurrences of the given substring and returns the modified string. */
    EXPORT string stringReplace(string str, string from, string to);

    /* Returns the amount of occurrences of a substring. */
    EXPORT int stringGetCount(string str, string sub, int index = 0);

    /* Converts a value into a string. */
    template<typename T> string toString(T val)
    {
        std::stringstream ss;
        ss << val;
        return ss.str();
    }

    /* Converts a value into a string with a given amount of decimal places. */
    template<typename T> string toStringPrec(T val, int prec)
    {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(prec) << val;
        return ss.str();
    }
}