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
#include "file/filefunc.hpp"
#include "file/json.hpp"
#include "file/resourcehandler.hpp"
#include "file/zipfunc.hpp"
#include "input/keyboardfunc.hpp"
#include "input/mousefunc.hpp"
#include "render/color.hpp"
#include "render/shader.hpp"
#include "render/font.hpp"
#include "render/image.hpp"
#include "render/drawfunc.hpp"
#include "util/data/list.hpp"
#include "util/data/mat4x4.hpp"
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