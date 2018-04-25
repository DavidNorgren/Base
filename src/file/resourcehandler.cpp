#define ZIP_STATIC
#include "zip.h"
#include <boost/filesystem.hpp>

#include "common.hpp"
#include "file/resourcehandler.hpp"
#include "file/filefunc.hpp"
#include "render/font.hpp"
#include "render/image.hpp"
#include "render/shader.hpp"
#include "scene/model.hpp"


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
    zip_source_t *src;
    zip_t *za;
    zip_error_t error;
    
    // Load from an external or internal zip archive
    #ifdef DYNAMIC_RESOURCES

    src = zip_source_file_create(DYNAMIC_RESOURCES_ZIP, 0, -1, &error);
    cout << "Loading resources from " << DYNAMIC_RESOURCES_ZIP << "..." << endl;

    #else

    if ((uint)resSize == 0)
        return;

    src = zip_source_buffer_create(resData, (uint)resSize, 0, &error);
    cout << "Loading resources from memory..." << endl;

    #endif

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
        string filename = zip_get_name(za, i, ZIP_FL_ENC_RAW);
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
            if (resMap.find(filename) == resMap.end())
            {
                Resource* res;
                string ext = fileGetExtension(filename);

                if (ext == ".ttf")
                    res = new Font(resData);
                else if (ext == ".glsl")
                    res = new Shader(resData);
                else if (ext == ".obj")
                    res = new Model(resData);
                else if (ext == ".png" || ext == ".jpg")
                    res = new Image(resData);
                else
                    res = new TextFile(resData);

                // Add to map
                resMap[filename] = res;
                cout << "\tADDED: " << filename << " (" << resData.size << " bytes)" << endl;
            }

            // Update existing
            else
            {
                if (resMap[filename]->reload(resData))
                    cout << "\tUPDATED: " << filename << " (" << resData.size << " bytes)" << endl;
            }
        }
        catch (ResourceLoadException e)
        {
            cout << "Resource load exception for " << filename << ":\n" << e.what() << endl;
        }

        delete resData.ptr;
        
    }
    
    zip_close(za);
}

void Base::ResourceHandler::checkReload()
{
    #ifdef DYNAMIC_RESOURCES
    if (!boost::filesystem::exists(DYNAMIC_RESOURCES_ZIP))
        return;
    
    uint lastMod = (uint)boost::filesystem::last_write_time(DYNAMIC_RESOURCES_ZIP);
    if (lastMod > zipLastModified && zipLastModified > 0)
    {
        cout << "UPDATING: last modified: " << zipLastModified << std::flush << endl;
        load();
    }

    zipLastModified = lastMod;
    #endif
}