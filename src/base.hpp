/*
 * 
 *  Base - Collection of base functionality for awesome projects
 * 
 *  - OpenGL windows
 *  - Drawing functions
 *  - Audio playbac
 *  - Audio/Video generation
 *  - HTTP
 *  - Embedded resources
 *  - Filesystem operations
 *  - JSON in/out
 *  - Shaders
 *  - Fonts
 *  - Images
 *  - Vector and matrix math
 *  - Dialogues
 *  - Date
 *  - Strings
 *  - Zip/Unzip
 *  - Mouse/Keyboard input
 * 
 *  Author: David Norgren @ www.stuffbydavid.com
 * 
 */

#pragma once

#include "common.hpp"
#include "file/directorypath.hpp"
#include "file/filefunc.hpp"
#include "file/filepath.hpp"
#include "file/json.hpp"
#include "file/zipfunc.hpp"
#include "input/keyboardfunc.hpp"
#include "input/mousefunc.hpp"
#include "render/color.hpp"
#include "render/drawfunc.hpp"
#include "render/image.hpp"
#include "render/renderfunc.hpp"
#include "render/rendertarget.hpp"
#include "render/surface.hpp"
#include "resource/resource.hpp"
#include "resource/resourcehandler.hpp"
#include "resource/shader.hpp"
#include "resource/font.hpp"
#include "resource/sprite.hpp"
#include "resource/obj.hpp"
#include "scene/camera.hpp"
#include "scene/light.hpp"
#include "scene/material.hpp"
#include "scene/model.hpp"
#include "scene/object.hpp"
#include "scene/scene.hpp"
#include "scene/trianglemesh.hpp"
#include "util/data/list.hpp"
#include "util/data/mat4.hpp"
#include "util/data/region2d.hpp"
#include "util/data/size2d.hpp"
#include "util/data/size3d.hpp"
#include "util/data/vec2.hpp"
#include "util/data/vec3.hpp"
#include "util/mathfunc.hpp"
#include "util/datefunc.hpp"
#include "util/stringfunc.hpp"
#include "util/timer.hpp"
#include "util/utilfunc.hpp"
#include "window/dialogfunc.hpp"
#include "window/window.hpp"
#include "apphandler.hpp"