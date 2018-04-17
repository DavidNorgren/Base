#pragma once


namespace Base
{
    /* Represents a file of raw data stored in memory. */
    struct File
    {
        string name;
        void* loaded;
        char* rawData;
        size_t size;
    };
}