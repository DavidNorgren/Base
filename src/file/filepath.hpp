#pragma once

#include "file/directorypath.hpp"


namespace Base
{
    class FilePath
    {
      public:

        // Constructors

        FilePath() {};
        
        FilePath(const string& fileName);
        
        // Binary operators
        
        FilePath& operator = (const FilePath& other)
        {
            fileName = other.fileName;
            return *this;
        }

        // Methods

        /* Returns the file path relative to a directory. */
        inline string getRelativePath(DirectoryPath directory) const;

        /* Returns the name of the file only, including the extension. */
        string getName() const;

        /* Returns the extension of the file path, including the leading dot. */
        string getExtension() const;

        /* Returns a new path with a new extension. */
        FilePath setExtension(const string& newExt) const;

        /* Returns the path to the directory containing the file. */
        DirectoryPath getDirectoryPath() const;

        /* Returns the full path and name to the file. */
        const string& getFullPath() const  { return fileName; }

      protected:
        string fileName;
    };

    // Conver to string
    
    inline std::ostream& operator << (std::ostream& out, const FilePath& file)
    {
        out << file.getFullPath() << endl;
    }
}