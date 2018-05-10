#pragma once

#include "util/data/list.hpp"


namespace Base
{
    /* Escapes quotations in the given string. */
    EXPORT string stringEscapeQuotes(const string& str);

    /* Converts a wide string to a UTF-8 encoded string and vice versa. */
    EXPORT string wstringToString(const wstring& str);
    EXPORT wstring stringToWstring(const string& wstr);

    /* Converts a string to lowercase. */
    EXPORT string stringGetLower(const string& str);

    /* Converts a string to uppercase. */
    EXPORT string stringGetUpper(const string& str);

    /* Checks whether two strings are equal, ignoring case. */
    EXPORT bool stringEqualsIgnoreCase(const string& a, const string& b);

    /* Creates a list of all the substrings in a string separated by a string. */
    EXPORT List<string> stringSplit(const string& str, const string& sep);
    
    /* Returns a substring of the given string. */
    EXPORT string stringSubstring(const string& str, int from, int length);

    /* Returns a new string with a certain substring removed. */
    EXPORT string stringErase(const string& str, int from, int length);

    /* Returns a new string with the given substring inserted at a position. */
    EXPORT string stringInsert(const string& str, const string& substr, int index);

    /* Returns the given string repeated a number of times.*/
    EXPORT string stringRepeat(const string& str, int count);

    /* Returns the lines in the given string as a list of strings.*/
    EXPORT List<string> stringGetLines(const string& str);

    /* Replaces all occurrences of the given substring and returns the modified string. */
    EXPORT string stringReplace(const string& str, const string& from, const string& to);

    /* Returns the amount of occurrences of a substring within the given string. */
    EXPORT int stringGetCount(const string& str, const string& sub, int index = 0);

    /* Converts a value into a string. */
    template<typename T> string toString(const T& val)
    {
        std::stringstream ss;
        ss << val;
        return ss.str();
    }

    /* Converts a value into a string with a given amount of decimal places. */
    template<typename T> string toStringPrec(const T& val, int prec)
    {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(prec) << val;
        return ss.str();
    }
}