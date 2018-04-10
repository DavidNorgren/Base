#pragma once

#ifdef _WIN32
    #define SLASH "\\"
#else
    #define SLASH "/"
#endif

#define DOT "."

#include <boost/filesystem.hpp>

#include <fstream>
#include <sys/stat.h>

#include "util/stringfunc.hpp"

namespace Base
{
    /* Represents a file stored in memory. */
    struct File
    {
        string name;
        void* loaded;
        char* rawData;
        size_t size;
    };

    /* Returns whether the given file exists. */
    bool fileExists(string filename);

    /* Returns the size of the given file, in bytes. */
    int fileGetSize(string filename);

    /* Returns the name of the file, without the path. */
    string fileGetName(string filename);

    /* Returns the path to the given file, including the final slash. */
    string fileGetPath(string filename);

    /* Returns the directory of the given file, not including the final slash. */
    string fileGetDirectory(string filename);

    /* Returns the extension of the file, including the leading dot. */
    string fileGetExtension(string filename);

    /* Returns the filename with the new extension (extension must include the leading dot). */
    string fileSetExtension(string filename, string ext);

    /* Returns the contents of the given text file. */
    string fileGetContents(string filename);

    /* Returns whether the given directory exists. */
    bool directoryExists(string directory);
}