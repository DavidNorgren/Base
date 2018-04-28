#pragma once

#include "file/directorypath.hpp"
#include "file/filepath.hpp"
#include "util/data/list.hpp"


namespace Base
{
    /* Returns whether the given file exists. */
    EXPORT bool fileExists(const FilePath& file);

    /* Returns the size of the given file, in bytes. */
    EXPORT int fileGetSize(const FilePath& file);

    /* Returns the timestamp when the file was last changed. */
    EXPORT uint fileGetLastChange(const FilePath& file);

    /* Returns the byte data o fhte file. */
    EXPORT FileData fileGetData(const FilePath& file);

    /* Returns the contents of the given text file as a single string. */
    EXPORT string fileGetText(const FilePath& file);

    /* Returns the contents of the given text file as a list of lines. */
    EXPORT List<string> fileGetLines(const FilePath& file);

    /* Returns whether the given directory exists. */
    EXPORT bool directoryExists(const DirectoryPath& directory);

    /* Returns a list of the files within the given directory. */
    EXPORT List<FilePath> directoryGetFiles(const DirectoryPath& directory, bool recurse = false, const string& filter = "");
}