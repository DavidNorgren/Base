#pragma once

#define MAX_FILENAME        1024
#define MAX_EXT             256
#define MAX_MULTIPLE        32

#define ENV_KDIALOG         0
#define ENV_APPLE           1

#include "util/data/list.hpp"


namespace Base
{
    /* Shows a file dialog for opening a file, starting at the given location ("" for last visited).
       Returns a list of the files selected. Use filters to select specific formats only. */
    EXPORT List<string> dialogOpenFile(string title, string location, List<string> filters, bool multiSelect = false);

    /* Shows a file dialog for saving a file, starting at the given location ("" for last visited).
       Use filters to select specific formats only. */
    EXPORT string dialogSaveFile(string title, string location, List<string> filters);
}