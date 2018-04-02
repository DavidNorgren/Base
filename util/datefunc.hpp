#pragma once

#include <time.h>
#include <stdio.h>

#include "util/stringfunc.hpp"


namespace Base
{
    /* Returns the current time as a string, formatted as YYYY-MM-DD HH:MM:SS */
    string getDateTimeNow();
}