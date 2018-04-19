#define ZIP_STATIC
#include "zip.h"

#include "common.hpp"
#include "file/resourcehandler.hpp"
#include "file/zipfunc.hpp"
#include "file/filefunc.hpp"
#include "render/font.hpp"
#include "render/image.hpp"
#include "render/shader.hpp"


EXPORT Base::ResourceHandler::ResourceHandler(void* data, size_t size)
{
    if (size == 0)
        return;
    
    zip_source_t *src;
    zip_t *za;
    zip_error_t error;
    
    src = zip_source_buffer_create(data, size, 0, &error);
    if (!src)
    {
        std::cout << "zip_source_buffer_create error: " << zip_error_strerror(&error) << std::endl;
        return;
    }
    
    za = zip_open_from_source(src, 0, &error);
    if (!za)
    {
        std::cout << "zip_open_from_source error: " << zip_error_strerror(&error) << std::endl;
        return;
    }
    
    int files_total = zip_get_num_files(za);
    
    std::cout << "Found resources: " << files_total << std::endl;
    
    for (int i = 0; i < files_total; i++)
    {
        zip_stat_t zs;
        zip_stat_index(za, i, 0, &zs);
        string filename = zip_get_name(za, i, ZIP_FL_ENC_RAW);
        zip_file *zf = zip_fopen_index(za, i, 0);
        if (!zf)
        {
            std::cout << "Could not open" << std::endl;
            continue;
        }
        
        File* f = new File();
        f->name = filename;
        f->size = zs.size;
        f->rawData = new char[f->size];
        
        if (zip_fread(zf, f->rawData, f->size) <= 0)
        {
            delete f;
            zip_fclose(zf);
            continue;
        }
        
        // Close file
        zip_fclose(zf);
        
        // Process into a manageable format
        string ext = fileGetExtension(filename);
        if (ext == FONTS_EXT)
            f->loaded = (void*)new Font(f, FONTS_SIZE, FONTS_START, FONTS_END);
        else if (ext == SHADERS_EXT)
            f->loaded = (void*)new Shader(f);
        else if (ext == IMAGES_EXT)
            f->loaded = (void*)new Image(f);
        
        // Add to map
        resMap[filename] = f;
        
        std::cout << "\t" << filename << " (" << f->size << " bytes)" << std::endl;
    }
    
    zip_close(za);
}

EXPORT Base::File* Base::ResourceHandler::find(string name)
{
    std::map<string, File*>::iterator i = resMap.find(name);
    if (i == resMap.end())
    {
        std::cout << "ResourceHandler: Could not find " << name << std::endl;
        exit(EXIT_FAILURE);
    }
    return i->second;
}