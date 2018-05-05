#pragma once

#include "file/filepath.hpp"
#include "util/timer.hpp"


namespace Base
{
    struct ResourceLoadException
    {
        ResourceLoadException(const string& message) : message(message) {};
        const string& what() const { return message; }

      private:
        string message;
    };

    /* A processed item in the "res" folder of the project. */
    class Resource
    {
      public:
        ~Resource() { cleanUp(); }

        /* Checks whether the file data should be loaded. */
        bool checkLoad();

        /* Creates a new resource. */
        static Base::Resource* createDynamic(const string& name, FilePath file);
        static Base::Resource* createInternal(const string& name, FileData data);

      protected:
        virtual void load(const FilePath& file) = 0;
        virtual void load(const FileData& file) = 0;

        bool isLoaded = false;

      private:
        virtual void cleanUp() {};
        static Base::Resource* create(const string& fileExt);

        FileData data;
        bool     isDynamic;
        FilePath dynamicFile;
        uint     dynamicLastChange;
    };
    
    /* A good ol' fashioned text file. */
    class TextFile : public Resource
    {
      public:
        TextFile() {};
        void load(const FilePath& file) override;
        void load(const FileData& data) override;
        string text;
    };
}