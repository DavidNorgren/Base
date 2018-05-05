#include <wchar.h>

#include "common.hpp"
#include "util/stringfunc.hpp"


EXPORT string Base::stringEscapeQuotes(const string& str)
{
    return stringReplace(str, "\"", "\\\"");
}

EXPORT string Base::wstringToString(const wstring& wstr)
{
    string result;
    result.resize(wstr.length());
    wcstombs(&result[0], &wstr[0], result.length());
    return result;
}

EXPORT wstring Base::stringToWstring(const string& str)
{
    wstring result;
    result.resize(str.length());
    mbstowcs(&result[0], &str[0], result.length());
    return result;
}

EXPORT Base::List<string> Base::stringSplit(const string& str, const string& sep)
{
    string nStr = str + sep;
    List<string> result;
    string::size_type pos = nStr.find(sep);
    string::size_type lastPos = 0;

    while (pos != string::npos)
    {
        result.add(nStr.substr(lastPos, pos - lastPos));
        lastPos = pos + sep.size();
        pos = nStr.find(sep, lastPos);
    }
    
    return result;
}

EXPORT string Base::stringReplace(const string& str, const string& from, const string& to)
{
    if (from.empty())
        return str;
    
    string nStr = str;
    size_t start_pos = 0;
    while((start_pos = nStr.find(from, start_pos)) != string::npos)
    {
        nStr.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    
    return nStr;
}

EXPORT string Base::stringSubstring(const string& str, int from, int length)
{
    return str.substr(from, length);
}

EXPORT string Base::stringErase(const string& str, int from, int length)
{
    string nStr = str;
    return nStr.erase(from, length);
}

EXPORT string Base::stringInsert(const string& str, const string& substr, int index)
{
    string nStr = str;
    return nStr.insert(index, substr);
}

EXPORT string Base::stringRepeat(const string& str, int count)
{
    string repeatStr = "";
    repeat (count)
        repeatStr += str;
    return repeatStr;
}

EXPORT int Base::stringGetCount(const string& str, const string& sub, int index)
{
    int count = 0;
    size_t pos = str.find(sub, index);
    
    while (pos != string::npos)
    {
        count++;
        pos = str.find(sub, pos + 1);
    }
    
    return count;
}

EXPORT Base::List<string> Base::stringGetLines(const string& str)
{
    std::stringstream sstream(str);
    List<string> contents;
    string line;
    
    while (getline(sstream, line))
        contents.add(line);
    
    return contents;
}