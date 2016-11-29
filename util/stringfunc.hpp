#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <string.h>
#include <wchar.h>
#include <stdlib.h>
#include <iomanip>

typedef std::string string;
typedef std::wstring wstring;

typedef std::vector<string> string_list;
typedef std::vector<wstring> wstring_list;


namespace Base
{
    
    /* Escapes quotations in the given string. */
    string stringEscapeQuotes(string str);

    /* Converts a wide string to a UTF-8 encoded string and vice versa. */
    string wstringToString(wstring str);
    wstring stringToWstring(string wstr);

    /* Creates a list of all the substrings in str separated by sep. */
    string_list stringSplit(string str, string sep);
    wstring_list wstringSplit(wstring wstr, wstring sep);

    /* Replaces all occurrences of the given substring and returns the modified string. */
    string stringReplace(string str, string from, string to);
    wstring wstringReplace(wstring wstr, wstring from, wstring to);

    /* Returns the amount of occurrences of a substring. */
    int stringGetCount(string str, string sub);
    int wstringGetCount(wstring wstr, wstring sub);

    /* Returns the width of a string when rendered. */
    int stringGetWidth(string str);

    /* Returns the height of a string when rendered. */
    int stringGetHeight(string str);

    /* Converts a float into a string with a given amount of decimal places. */
    string toStringPrec(float val, int prec);
    
}