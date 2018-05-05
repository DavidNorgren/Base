#include <boost/filesystem.hpp>

#include "common.hpp"
#include "resource/resource.hpp"
#include "resource/font.hpp"
#include "resource/sprite.hpp"
#include "resource/shader.hpp"
#include "resource/obj.hpp"
#include "resource/testscene.hpp"
#include "file/filefunc.hpp"

#ifdef DYNAMIC_RESOURCES
#include "windows.h" // Sleep()
#endif

namespace bfs = boost::filesystem;

Base::Resource* Base::Resource::createDynamic(const string& name, FilePath file)
{
    Resource* res = create(file.getExtension());
    res->isDynamic = true;
    res->dynamicFile = file;
    res->dynamicLastChange = 0;
    return res;
}

Base::Resource* Base::Resource::createInternal(const string& name, FileData data)
{
    Resource* res = create(FilePath(name).getExtension());
    res->isDynamic = false;
    res->data = data;
    return res;
}

Base::Resource* Base::Resource::create(const string& fileExt)
{
    if (fileExt == ".ttf")
        return new Font();
    else if (fileExt == ".glsl")
        return new Shader();
    else if (fileExt == ".png" ||
             fileExt == ".jpg")
        return new Sprite();
    else if (fileExt == ".obj")
        return new Obj();
    else if (fileExt == ".testscene")
        return new TestScene();
    else
        return new TextFile();
}

bool Base::Resource::checkLoad()
{
    // Dynamic resources are updated as soon as their disk counterpart is modified.
    if (isDynamic)
    {
        if (!fileExists(dynamicFile) || fileGetSize(dynamicFile) == 0)
            return false;

        if (!isLoaded)
        {
            load(dynamicFile);
            isLoaded = true;
            return true;
        }
        else
        {
            // Wait in a loop until successfully loaded and the method returns true
            bool isError = false;
            do
            {
                try
                {
                    // Check if file has been changed since last time
                    if (uint lastChange = fileGetLastChange(dynamicFile))
                    {
                        if (lastChange > dynamicLastChange && dynamicLastChange > 0)
                        {
                            // Add 10ms delay to allow file saving
                            #ifdef DYNAMIC_RESOURCES
                            Sleep(10);
                            #endif

                            dynamicLastChange = lastChange;
                            cleanUp();
                            isLoaded = false;
                            load(dynamicFile);
                            isLoaded = true;
                            return true;
                        }

                        dynamicLastChange = lastChange;
                    }
                }
                catch (const ResourceLoadException& ex)
                {
                    cout << "ResourceLoadException: " << ex.what() << endl;
                    isError = true;
                }
            }
            while (isError);
            
            return false;
        }
    }

    // Load from internal data (once only)
    else if (!isLoaded)
    {
        load(data);
        isLoaded = true;
    }

    return false;
}

void Base::TextFile::load(const FilePath& file)
{
    text = fileGetText(file);
}

void Base::TextFile::load(const FileData& data)
{
    text = string(&data[0], data.size());
}