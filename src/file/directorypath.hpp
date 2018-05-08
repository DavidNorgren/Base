#pragma once


namespace Base
{
    class FilePath;
    class DirectoryPath
    {
      public:

        // Constructors

        DirectoryPath() {}
        
        EXPORT DirectoryPath(const string& dirName);

        // Binary operators
        
        DirectoryPath& operator = (const DirectoryPath& other)
        {
            dirName = other.dirName;
            return *this;
        }

        // Methods

        /* Returns the path to a file in the directory. */
        EXPORT FilePath getFilePath(const string& name);

        /* Returns the full name and path to the directory. */
        const string& getFullPath() const { return dirName; }

      private:
        string dirName;
    };

    // Convert to string
    
    inline std::ostream& operator << (std::ostream& out, const DirectoryPath& directory)
    {
        out << directory.getFullPath() << endl;
    }
}