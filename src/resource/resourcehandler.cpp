#define ZIP_STATIC
#include "zip.h"
#include <boost/filesystem.hpp>

#include "common.hpp"
#include "resource/resourcehandler.hpp"
#include "file/filefunc.hpp"
#include "util/stringfunc.hpp"


// DEBUG!
//#define DYNAMIC_RESOURCES
//#define DYNAMIC_RESOURCES_DIR ""

#ifndef DYNAMIC_RESOURCES
extern char resData[] asm("_binary_res_zip_start");
extern char resSize[] asm("_binary_res_zip_size");
#endif

EXPORT Base::ResourceHandler::ResourceHandler()
{
    #ifdef DYNAMIC_RESOURCES
    cout << "[ResourceHandler] Loading dynamic resources from " << DYNAMIC_RESOURCES_DIR << endl;
    #else
    zip_source_t *src;
    zip_t *za;
    zip_error_t error;
    
    if ((uint)resSize == 0)
        return;

    src = zip_source_buffer_create(resData, (uint)resSize, 0, &error);
    cout << "[ResourceHandler] Loading resources from memory" << endl;

    if (!src)
    {
        cout << "zip_source_buffer_create error: " << zip_error_strerror(&error) << endl;
        return;
    }
    
    za = zip_open_from_source(src, 0, &error);
    if (!za)
    {
        cout << "zip_open_from_source error: " << zip_error_strerror(&error) << endl;
        return;
    }
    
    int files_total = zip_get_num_files(za);
    
    cout << "[ResourceHandler] Found resources: " << files_total << endl;
    
    for (int i = 0; i < files_total; i++)
    {
        zip_stat_t zs;
        zip_stat_index(za, i, 0, &zs);
        string name = zip_get_name(za, i, ZIP_FL_ENC_RAW);
        zip_file *zf = zip_fopen_index(za, i, 0);
        if (!zf)
        {
            cout << "Could not open" << endl;
            continue;
        }
        
        FileData data(zs.size);
        if (zip_fread(zf, &data[0], data.size()) <= 0)
        {
            zip_fclose(zf);
            continue;
        }
        
        // Close file
        zip_fclose(zf);
        
        // Add new resource and bind data
        resMap[name] = Resource::createInternal(name, data);
        cout << "\tADDED: " << name << " (" << data.size() << " bytes)" << endl;
    }
    
    zip_close(za);
    #endif
}

EXPORT Base::Resource* Base::ResourceHandler::get(const string& name)
{
    auto i = resMap.find(name);
    Resource* res = nullptr;

    if (i == resMap.end())
    {
    #ifdef DYNAMIC_RESOURCES
        FilePath file = DirectoryPath(DYNAMIC_RESOURCES_DIR).getFilePath(name);
        if (fileExists(file))
        {
            // Add new resource and bind filename
            res = Resource::createDynamic(name, file);
            resMap[name] = res;
            cout << "\tADDED: " << name << endl;
        }
        else
    #endif
        throw ResourceException("Could not find " + name);
    }
    else
        res = i->second;
    
    // Process if not already loaded or re-load if the file was changed (dynamic resources only)
    try
    {
        if (res->checkLoad())
            cout << "[ResourceHandler] LOADED " << name << endl << std::flush;
    }
    catch (runtime_error e)
    {
        cout << "Resource load exception for " << name << ":\n" << e.what() << endl << std::flush;
    }

    return res;
}