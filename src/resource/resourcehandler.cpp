#define ZIP_STATIC
#include "zip.h"
#include <boost/filesystem.hpp>

#include "common.hpp"
#include "resource/resourcehandler.hpp"
#include "file/filefunc.hpp"
#include "util/stringfunc.hpp"


#ifndef DYNAMIC_RESOURCES
extern char resData[] asm("_binary_res_zip_start");
extern char resSize[] asm("_binary_res_zip_size");
#endif

EXPORT Base::ResourceHandler::ResourceHandler()
{
    load();
}

EXPORT Base::Resource* Base::ResourceHandler::get(string name)
{
    Map<string, Resource*>::iterator i = resMap.find(name);
    if (i == resMap.end())
        throw ResourceException("Could not find " + name);
    
    return i->second;
}

void Base::ResourceHandler::load()
{
    #ifdef DYNAMIC_RESOURCES
    cout << "Loading dynamic resources: " << DYNAMIC_RESOURCES_DIR << endl;

    List<string> files = directoryGetFiles(DYNAMIC_RESOURCES_DIR, true);
    for (const string& filename : files)
    {
        // Get resource name
        string name = stringReplace(filename, DYNAMIC_RESOURCES_DIR, "");

        try
        {
            // Add to map
            resMap[name] = Resource::create(filename);
            resMap[name]->dynamicFilename = filename;
            cout << "\tADDED: " << name << endl;
        }
        catch (ResourceLoadException e)
        {
            cout << "Resource load exception for " << name << ":\n" << e.what() << endl;
        }
    }

    #else
    zip_source_t *src;
    zip_t *za;
    zip_error_t error;
    
    if ((uint)resSize == 0)
        return;

    src = zip_source_buffer_create(resData, (uint)resSize, 0, &error);
    cout << "Loading resources from memory..." << endl;

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
    
    cout << "Found resources: " << files_total << endl;
    
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
        
        Data resData;
        resData.size = zs.size;
        resData.ptr = new char[zs.size];

        if (zip_fread(zf, resData.ptr, resData.size) <= 0)
        {
            delete resData.ptr;
            zip_fclose(zf);
            continue;
        }
        
        // Close file
        zip_fclose(zf);
        
        // Process into a new resource
        try
        {
            // Add to map
            resMap[name] = Resource::create(fileGetExtension(name), resData);
            cout << "\tADDED: " << name << " (" << resData.size << " bytes)" << endl;
        }
        catch (ResourceLoadException e)
        {
            cout << "Resource load exception for " << name << ":\n" << e.what() << endl;
        }

        delete resData.ptr;
    }
    
    zip_close(za);
    #endif
}

void Base::ResourceHandler::checkReload()
{
    #ifdef DYNAMIC_RESOURCES
    
    for (Map<string, Resource*>::iterator it = resMap.begin(); it != resMap.end(); it++)
    {
        const string& name = it->first;
        Resource* res = it->second;

        try
        {
            if (res->checkReload())
                cout << "UPDATED " << name << endl;
        }
        catch (ResourceLoadException e)
        {
            cout << "Resource load exception for " << name << ":\n" << e.what() << endl;
        }
    }

    #endif
}