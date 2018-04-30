#pragma once

#include "util/stringfunc.hpp"


namespace Base
{
    class FilePath;
    class DirectoryPath
    {
      public:
        DirectoryPath(const string& dirName)
        {
            this->dirName = stringReplace(dirName, "\\", "/");      // Use forward-slash
            if (this->dirName[this->dirName.length() - 1] == '/')   // Remove trailing slash
                this->dirName = this->dirName.substr(0, this->dirName.length() - 1);
        };
        
        inline DirectoryPath& operator = (const DirectoryPath& other)
        {
            dirName = other.dirName;
            return *this;
        }

        /* Returns the full name and path to the directory. */
        inline const string& getFullPath() const
        {
            return dirName;
        }

        /* Returns the path to a file in the directory. */
        inline FilePath getFilePath(const string& name);

      private:
        string dirName;
    };
    
    inline std::ostream& operator << (std::ostream& out, const DirectoryPath& directory)
    {
        out << directory.getFullPath() << endl;
    }
}