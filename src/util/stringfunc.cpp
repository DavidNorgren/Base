#include "util/stringfunc.hpp"


string Base::stringEscapeQuotes(string str)
{
    return stringReplace(str, "\"", "\\\"");
}

string Base::wstringToString(wstring wstr)
{
    string result;
    result.resize(wstr.length());
    wcstombs(&result[0], &wstr[0], result.length());
    return result;
}

wstring Base::stringToWstring(string str)
{
    wstring result;
    result.resize(str.length());
    mbstowcs(&result[0], &str[0], result.length());
    return result;
}

std::vector<string> Base::stringSplit(string str, string sep)
{
    str += sep;
    std::vector<string> result;
    string::size_type pos = str.find(sep);
    string::size_type lastPos = 0;

    while (pos != string::npos)
    {
        result.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = pos + sep.size();
        pos = str.find(sep, lastPos);
    }
    
    return result;
}

string Base::stringReplace(string str, string from, string to)
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

string Base::stringSubstring(string str, int from, int length)
{
    return str.substr(from, length);
}

string Base::stringErase(string str, int from, int length)
{
    return str.erase(from, length);
}

string Base::stringInsert(string str, string substr, int index)
{
    return str.insert(index, substr);
}

string Base::stringRepeat(string str, int count)
{
    string repeatStr = "";
    for (int i = 0; i < count; i++)
        repeatStr += str;
    return repeatStr;
}

int Base::stringGetCount(string str, string sub, int index)
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