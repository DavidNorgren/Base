#include "common.hpp"
#include "file/filepath.hpp"
#include "util/stringfunc.hpp"


EXPORT Base::FilePath::FilePath(const string& fileName)
{
    this->fileName = stringReplace(fileName, "\\", "/");
}

EXPORT string Base::FilePath::getRelativePath(Base::DirectoryPath directory) const
{
    return stringReplace(fileName, directory.getFullPath() + "/", "");
}

EXPORT string Base::FilePath::getName() const
{
    size_t pos = fileName.find_last_of("/");

    if (pos == string::npos)
        return fileName;

    return fileName.substr(pos + 1, fileName.length() - pos - 1);
}

EXPORT string Base::FilePath::getExtension() const
{
    string name = getName();
    size_t pos = name.find_last_of('.');

    if (pos == string::npos)
        return "";

    size_t extLen = name.length() - pos;
    return fileName.substr(fileName.length() - extLen, extLen);
}

EXPORT Base::FilePath Base::FilePath::setExtension(const string& newExt) const
{
    string name = getName();
    size_t pos = name.find_last_of('.');

    if (pos == string::npos)
        return FilePath(fileName);
    
    size_t extLen = name.length() - pos;
    return FilePath(fileName.substr(0, fileName.length() - extLen) + newExt);
}

EXPORT Base::DirectoryPath Base::FilePath::getDirectoryPath() const
{
    size_t pos = fileName.find_last_of('/');

    if (pos == string::npos)
        return DirectoryPath("");

    return DirectoryPath(fileName.substr(0, pos));
}