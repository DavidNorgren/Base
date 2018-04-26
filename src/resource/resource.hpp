#pragma once


namespace Base
{
    struct ResourceLoadException : public runtime_error
    {
        ResourceLoadException(const string& message) : runtime_error(message) {};
    };

    /* A processed item in the "res" folder of the project. */
    class ResourceHandler;
    class Resource
    {
      friend ResourceHandler;

      public:
        /* Implemented by reload-able children only. Returns whether
           the resource was successfully reloaded. */ 
        virtual bool reload(const string& filename) { return false; };

        /* Creates a new resource from external or internal data. */
        static Base::Resource* create(const string& filename);
        static Base::Resource* create(const string& fileExtension, const Data& data);

      protected:
        bool    checkReload();
        string  dynamicFilename;
        uint    dynamicLastTime = 0;
    };
    
    /* A good ol' fashioned text file. */
    class TextFile : public Resource
    {
      public:
        TextFile(const string& text)              { this->text = text; }
        TextFile(const Data& data)                { text = string(data.ptr, data.size); }
        bool reload(const string& text) override  { this->text = text; }
        string text;
    };
}