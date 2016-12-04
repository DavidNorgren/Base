#include "util/stringfunc.hpp"
#include "apphandler.hpp"


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


string_list Base::stringSplit(string str, string sep)
{
    str += sep;
    string_list result;
    string::size_type pos = str.find(sep);
    string::size_type lastPos = 0;

    while (pos != string::npos) {
        result.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = pos + sep.size();
        pos = str.find(sep, lastPos);
    }
    
    return result;
}


wstring_list Base::stringSplit(wstring wstr, wstring sep)
{
    wstring_list result;
    string::size_type lastPos = wstr.find_first_not_of(sep, 0);
    string::size_type pos     = wstr.find_first_of(sep, lastPos);

    while (wstring::npos != pos || wstring::npos != lastPos) {
        result.push_back(wstr.substr(lastPos, pos - lastPos));
        lastPos = wstr.find_first_not_of(sep, pos);
        pos = wstr.find_first_of(sep, lastPos);
    }
    
    return result;
}


string Base::stringReplace(string str, string from, string to)
{
    if (from.empty()) {
        return str;
    }
    
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    
    return str;
}


wstring Base::stringReplace(wstring wstr, wstring from, wstring to)
{
    if (from.empty()) {
        return wstr;
    }
    
    size_t start_pos = 0;
    while((start_pos = wstr.find(from, start_pos)) != wstring::npos) {
        wstr.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    
    return wstr;
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


int Base::stringGetCount(string str, string sub, int index)
{
    int count = 0;
    size_t pos = str.find(sub, index);
    
    while(pos != string::npos) {
        count++;
        pos = str.find(sub, pos + 1);
    }
    
    return count;
}


int Base::stringGetCount(wstring wstr, wstring sub, int index)
{
    int count = 0;
    size_t pos = wstr.find(sub, index);
    
    while(pos != string::npos) {
        count++;
        pos = wstr.find(sub, pos + 1);
    }
    
    return count;
}


int Base::stringGetWidth(string str)
{
    return appHandler->drawingFont->stringGetWidth(str);
}


int Base::stringGetHeight(string str)
{
    return appHandler->drawingFont->stringGetHeight(str);
}


string Base::toStringPrec(float val, int prec)
{
    std::stringstream ss;
    ss << std::setprecision(prec) << val;
    return ss.str();
}    