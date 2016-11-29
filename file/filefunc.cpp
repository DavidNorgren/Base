#include "file/filefunc.hpp"


bool Base::fileExists(wstring filename)
{
    std::ifstream file(wstringToString(filename));
    return (bool)file;
}


int Base::fileGetSize(wstring filename)
{
    std::ifstream file(wstringToString(filename), std::ios::binary | std::ios::ate);
    return file.tellg();
}


wstring Base::fileGetName(wstring filename)
{
    size_t pos = filename.find_last_of(SLASH);

    if (pos == string::npos) {
        return filename;
    }

    return filename.substr(pos + 1, filename.size() - pos - 1);
}


wstring Base::fileGetPath(wstring filename)
{
    size_t pos = filename.find_last_of(SLASH);

    if (pos == string::npos) {
        return filename;
    }

    return filename.substr(0, pos + 1);
}


wstring Base::fileGetDirectory(wstring filename)
{
    size_t pos = filename.find_last_of(SLASH);

    if (pos == string::npos) {
        return filename;
    }

    return filename.substr(0, pos);
}


wstring Base::fileGetExtension(wstring filename)
{
    wstring fn = fileGetName(filename);
    size_t pos = fn.find_last_of(DOT);

    if (pos == string::npos) {
        return L"";
    }

    return fn.substr(pos, fn.size() - pos);
}


wstring Base::fileSetExtension(wstring filename, wstring ext)
{
    wstring fn = fileGetName(filename);
    wstring fp = fileGetPath(filename);
    size_t pos = fn.find_last_of(DOT);

    if (pos != string::npos) {
        fn = fn.substr(0, pos);
    }
    
    return fp + fn + ext;
}


wstring Base::fileGetContents(wstring filename)
{
    std::ifstream file(wstringToString(filename));
    string line, contents = "";
    
    while (getline(file, line)) {
        contents += line + '\n';
    }
    
    file.close();
    
    return stringToWstring(contents);
}


bool Base::directoryExists(wstring directory)
{
    struct stat info;
    
    if (stat(&wstringToString(directory)[0], &info) != 0) {
        return false;
    }

    return (info.st_mode & S_IFDIR);
}