/*
 * 
 *  Base - Collection of base functionality for awesome projects
 * 
 *  - OpenGL window
 *  - Drawing functions
 *  - Audio playback (TODO)
 *  - Audio/Video generation (TODO)
 *  - HTTP (TODO)
 *  - Embedded resources
 *  - File handling
 *  - Shaders
 *  - Fonts
 *  - Images
 *  - Vector and matrix math
 *  - Dialogues (WIP)
 *  - Date (TODO)
 *  - Strings
 *  - Zip/Unzip (WIP)
 *  - Mouse/Keyboard input
 * 
 *  Author: David Norgren @ www.stuffbydavid.com
 * 
 */

#pragma once

#include "file/filefunc.hpp"
#include "file/resourcehandler.hpp"
#include "file/zipfunc.hpp"
#include "input/keyboardfunc.hpp"
#include "input/mousefunc.hpp"
#include "render/color.hpp"
#include "render/shader.hpp"
#include "render/font.hpp"
#include "render/image.hpp"
#include "render/drawfunc.hpp"
#include "util/math/vec2.hpp"
#include "util/math/vec3.hpp"
#include "util/math/mat4x4.hpp"
#include "util/math/mathfunc.hpp"
#include "util/datefunc.hpp"
#include "util/stringfunc.hpp"
#include "util/utilfunc.hpp"
#include "window/dialogfunc.hpp"
#include "window/window.hpp"
#include "apphandler.hpp"

using namespace Base;