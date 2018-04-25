#include <boost/filesystem.hpp>
#include <sys/stat.h>

#include "common.hpp"
#include "file/filefunc.hpp"
#include "util/stringfunc.hpp"

#ifdef _WIN32
    #define SLASH "\\"
#else
    #define SLASH "/"
#endif

#define DOT "."


EXPORT bool Base::fileExists(string filename)
{
    return boost::filesystem::exists(filename);
}

EXPORT int Base::fileGetSize(string filename)
{
    return boost::filesystem::file_size(filename);
}

EXPORT string Base::fileGetName(string filename)
{
    size_t pos = filename.find_last_of(SLASH);

    if (pos == string::npos)
        return filename;

    return filename.substr(pos + 1, filename.size() - pos - 1);
}

EXPORT string Base::fileGetPath(string filename)
{
    size_t pos = filename.find_last_of(SLASH);

    if (pos == string::npos)
        return filename;

    return filename.substr(0, pos + 1);
}

EXPORT string Base::fileGetDirectory(string filename)
{
    size_t pos = filename.find_last_of(SLASH);

    if (pos == string::npos)
        return filename;

    return filename.substr(0, pos);
}

EXPORT string Base::fileGetExtension(string filename)
{
    string fn = fileGetName(filename);
    size_t pos = fn.find_last_of(DOT);

    if (pos == string::npos)
        return "";

    return fn.substr(pos, fn.size() - pos);
}

EXPORT string Base::fileSetExtension(string filename, string ext)
{
    string fn = fileGetName(filename);
    string fp = fileGetPath(filename);
    size_t pos = fn.find_last_of(DOT);

    if (pos != string::npos)
        fn = fn.substr(0, pos);
    
    return fp + fn + ext;
}

EXPORT string Base::fileGetContents(string filename)
{
    std::ifstream file(filename);
    string line, contents = "";
    
    while (getline(file, line))
        contents += line + '\n';
    
    file.close();
    
    return contents;
}

EXPORT bool Base::directoryExists(string directory)
{
    return boost::filesystem::exists(directory);
}