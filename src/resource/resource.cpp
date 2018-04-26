#include <boost/filesystem.hpp>

#include "common.hpp"
#include "resource/resource.hpp"
#include "resource/font.hpp"
#include "resource/image.hpp"
#include "resource/shader.hpp"
#include "resource/model.hpp"
#include "file/filefunc.hpp"


namespace bfs = boost::filesystem;

Base::Resource* Base::Resource::create(const string& filename)
{
    string ext = fileGetExtension(filename);
    if (ext == ".ttf")
        return new Font(filename);
    else if (ext == ".glsl")
        return new Shader(filename);
    else if (ext == ".obj")
        return new Model(filename);
    else if (ext == ".png" || ext == ".jpg")
        return new Image(filename);
    else
        return new TextFile(filename);
}

Base::Resource* Base::Resource::create(const string& fileExtension, const Data& data)
{
    if (fileExtension == ".ttf")
        return new Font(data);
    else if (fileExtension == ".glsl")
        return new Shader(data);
    else if (fileExtension == ".obj")
        return new Model(data);
    else if (fileExtension == ".png" || fileExtension == ".jpg")
        return new Image(data);
    else
        return new TextFile(data);
}

bool Base::Resource::checkReload()
{
    if (!boost::filesystem::exists(dynamicFilename))
        return false;
        
    uint lastMod = (uint)boost::filesystem::last_write_time(dynamicFilename);
    if (lastMod > dynamicLastTime && dynamicLastTime > 0)
    {
        dynamicLastTime = lastMod;
        return reload(dynamicFilename);
    }

    dynamicLastTime = lastMod;
    return false;
}