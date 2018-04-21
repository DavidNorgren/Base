#include <wchar.h>

#include "common.hpp"
#include "util/stringfunc.hpp"


EXPORT string Base::stringEscapeQuotes(string str)
{
    return stringReplace(str, "\"", "\\\"");
}

EXPORT string Base::wstringToString(wstring wstr)
{
    string result;
    result.resize(wstr.length());
    wcstombs(&result[0], &wstr[0], result.length());
    return result;
}

EXPORT wstring Base::stringToWstring(string str)
{
    wstring result;
    result.resize(str.length());
    mbstowcs(&result[0], &str[0], result.length());
    return result;
}

EXPORT Base::List<string> Base::stringSplit(string str, string sep)
{
    str += sep;
    List<string> result;
    string::size_type pos = str.find(sep);
    string::size_type lastPos = 0;

    while (pos != string::npos)
    {
        result.add(str.substr(lastPos, pos - lastPos));
        lastPos = pos + sep.size();
        pos = str.find(sep, lastPos);
    }
    
    return result;
}

EXPORT string Base::stringReplace(string str, string from, string to)
{
    if (from.empty())
        return str;
    
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    
    return str;
}

EXPORT string Base::stringSubstring(string str, int from, int length)
{
    return str.substr(from, length);
}

EXPORT string Base::stringErase(string str, int from, int length)
{
    return str.erase(from, length);
}

EXPORT string Base::stringInsert(string str, string substr, int index)
{
    return str.insert(index, substr);
}

EXPORT string Base::stringRepeat(string str, int count)
{
    string repeatStr = "";
    repeat (count)
        repeatStr += str;
    return repeatStr;
}

EXPORT int Base::stringGetCount(string str, string sub, int index)
{
    int count = 0;
    size_t pos = str.find(sub, index);
    
    while(pos != string::npos)
    {
        count++;
        pos = str.find(sub, pos + 1);
    }
    
    return count;
}