#include <boost/filesystem.hpp>
#include <sys/stat.h>

#include "common.hpp"
#include "file/filefunc.hpp"
#include "util/stringfunc.hpp"


namespace bfs = boost::filesystem;

string convertSlash(const string& filename)
{
    return Base::stringReplace(filename, "\\", "/");
}

EXPORT bool Base::fileExists(string filename)
{
    return bfs::exists(filename);
}

EXPORT int Base::fileGetSize(string filename)
{
    return bfs::file_size(filename);
}

EXPORT string Base::fileGetName(string filename)
{
    size_t pos = filename.find_last_of("/");

    if (pos == string::npos)
        return filename;

    return filename.substr(pos + 1, filename.size() - pos - 1);
}

EXPORT string Base::fileGetPath(string filename)
{
    size_t pos = filename.find_last_of('/');

    if (pos == string::npos)
        return filename;

    return filename.substr(0, pos + 1);
}

EXPORT string Base::fileGetDirectory(string filename)
{
    size_t pos = filename.find_last_of('/');

    if (pos == string::npos)
        return filename;

    return filename.substr(0, pos);
}

EXPORT string Base::fileGetExtension(string filename)
{
    string fn = fileGetName(filename);
    size_t pos = fn.find_last_of('.');

    if (pos == string::npos)
        return "";

    return fn.substr(pos, fn.size() - pos);
}

EXPORT string Base::fileSetExtension(string filename, string ext)
{
    string fn = fileGetName(filename);
    string fp = fileGetPath(filename);
    size_t pos = fn.find_last_of('.');

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
    return bfs::exists(directory);
}

EXPORT Base::List<string> Base::directoryGetFiles(string directory, bool recurse, string filter)
{
    List<string> files;
    bfs::path dirPath(directory);
    bfs::directory_iterator end_itr;

    for (bfs::directory_iterator itr(dirPath); itr != end_itr; ++itr)
    {
        bfs::path curPath = itr->path();
        if (bfs::is_directory(curPath) && recurse)
            files.add(directoryGetFiles(curPath.string(), true, filter));
        else if (bfs::is_regular_file(curPath))
            files.add(convertSlash(curPath.string()));
    }

    return files;
}