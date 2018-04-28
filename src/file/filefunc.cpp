#include <boost/filesystem.hpp>

#include "common.hpp"
#include "file/filefunc.hpp"
#include "util/stringfunc.hpp"


namespace bfs = boost::filesystem;

EXPORT bool Base::fileExists(const Base::FilePath& file)
{
    return bfs::exists(file.getFullPath());
}

EXPORT int Base::fileGetSize(const Base::FilePath& file)
{
    return bfs::file_size(file.getFullPath());
}

EXPORT uint Base::fileGetLastChange(const Base::FilePath& file)
{
    return (uint)boost::filesystem::last_write_time(file.getFullPath());
}

EXPORT FileData Base::fileGetData(const Base::FilePath& file)
{
    std::ifstream ifs(file.getFullPath(), std::ios::binary | std::ios::ate);
    std::ifstream::pos_type pos = ifs.tellg();

    FileData data(pos);
    ifs.seekg(0, std::ios::beg);
    ifs.read(&data[0], pos);

    return data;
}

EXPORT string Base::fileGetText(const Base::FilePath& file)
{
    std::ifstream ifs(file.getFullPath());
    string line, contents = "";
    
    while (getline(ifs, line))
        contents += line + '\n';
    
    ifs.close();
    return contents;
}

EXPORT Base::List<string> Base::fileGetLines(const Base::FilePath& file)
{
    std::ifstream ifs(file.getFullPath());
    List<string> lines;
    string line;
    
    while (getline(ifs, line))
        lines.add(line);
    
    ifs.close();
    
    return lines;
}

EXPORT bool Base::directoryExists(const Base::DirectoryPath& directory)
{
    return bfs::exists(directory.getFullPath());
}

EXPORT Base::List<Base::FilePath> Base::directoryGetFiles(const Base::DirectoryPath& directory, bool recurse, const string& filter)
{
    List<FilePath> files;
    bfs::path dirPath(directory.getFullPath());
    bfs::directory_iterator end_itr;

    for (bfs::directory_iterator itr(dirPath); itr != end_itr; ++itr)
    {
        bfs::path curPath = itr->path();
        if (bfs::is_directory(curPath) && recurse)
            files.addList(directoryGetFiles(DirectoryPath(curPath.string()), true, filter));
        else if (bfs::is_regular_file(curPath))
            files.add(FilePath(curPath.string()));
    }

    return files;
}