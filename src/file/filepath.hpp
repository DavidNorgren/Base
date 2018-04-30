#pragma once

#include "file/directorypath.hpp"
#include "util/stringfunc.hpp"


namespace Base
{
    class FilePath
    {
      public:
        FilePath() {};
        
        FilePath(const string& fileName)
        {
            this->fileName = stringReplace(fileName, "\\", "/");
        }
        
        inline FilePath& operator = (const FilePath& other)
        {
            fileName = other.fileName;
            return *this;
        }

        /* Returns the full path and name to the file. */
        inline const string& getFullPath() const
        {
            return fileName;
        }

        /* Returns the file path relative to a directory. */
        inline string getRelativePath(DirectoryPath directory) const
        {
            return stringReplace(fileName, directory.getFullPath() + "/", "");
        }

        /* Returns the name of the file only, including the extension. */
        string getName() const
        {
            size_t pos = fileName.find_last_of("/");

            if (pos == string::npos)
                return fileName;

            return fileName.substr(pos + 1, fileName.length() - pos - 1);
        }

        /* Returns the extension of the file path, including the leading dot. */
        string getExtension() const
        {
            string name = getName();
            size_t pos = name.find_last_of('.');

            if (pos == string::npos)
                return "";

            size_t extLen = name.length() - pos;
            return fileName.substr(fileName.length() - extLen, extLen);
        }

        /* Returns a new path with a new extension. */
        FilePath setExtension(const string& newExt) const
        {
            string name = getName();
            size_t pos = name.find_last_of('.');

            if (pos == string::npos)
                return FilePath(fileName);
            
            size_t extLen = name.length() - pos;
            return FilePath(fileName.substr(0, fileName.length() - extLen) + newExt);
        }

        /* Returns the path to the directory containing the file. */
        DirectoryPath getDirectoryPath() const
        {
            size_t pos = fileName.find_last_of('/');

            if (pos == string::npos)
                return DirectoryPath("");

            return DirectoryPath(fileName.substr(0, pos));
        }

      protected:
        string fileName;
    };
    
    inline std::ostream& operator << (std::ostream& out, const FilePath& file)
    {
        out << file.getFullPath() << endl;
    }

    inline FilePath DirectoryPath::getFilePath(const string& name)
    {
        return FilePath(dirName + "/" + name);
    }
}