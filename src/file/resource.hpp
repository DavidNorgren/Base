#pragma once


namespace Base
{
    struct ResourceLoadException : public runtime_error
    {
        ResourceLoadException(string message) : runtime_error(message) {};
    };
    
    /* A pointer to raw byte data. */
    struct Data
    {
        char* ptr;
        uint size;
    };

    /* A processed item in the "res" folder of the project. */
    class Resource
    {
      public:
        /* Implemented by reload-able children only. Returns whether
           the resource was successfully reloaded. */ 
        virtual bool reload(const Data& data) { return false; };
    };
    
    /* A good ol' fashioned text file. */
    class TextFile : public Resource
    {
      public:
        TextFile(const Data& data)              { text = string(data.ptr, data.size); }
        bool reload(const Data& data) override  { text = string(data.ptr, data.size); return true; }
        string text;
    };
}