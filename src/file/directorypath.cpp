#include "common.hpp"
#include "util/stringfunc.hpp"
#include "file/directorypath.hpp"
#include "file/filepath.hpp"


EXPORT Base::DirectoryPath::DirectoryPath(const string& dirName)
{
    this->dirName = stringReplace(dirName, "\\", "/");      // Use forward-slash
    if (this->dirName[this->dirName.length() - 1] == '/')   // Remove trailing slash
        this->dirName = this->dirName.substr(0, this->dirName.length() - 1);
};

EXPORT Base::FilePath Base::DirectoryPath::getFilePath(const string& name)
{
    return FilePath(dirName + "/" + name);
}