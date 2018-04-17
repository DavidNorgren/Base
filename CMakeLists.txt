cmake_minimum_required(VERSION 3.7)

# Set project
project(base)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-g")
add_definitions("-DEXPORT=__declspec(dllexport)")
set(LIBS_DIR "C:/Dev/Libs")

# Add Base sources
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")
file(GLOB_RECURSE SRCS "src/*.cpp")
add_library(base SHARED ${SRCS})

# Add Boost filesystem & system
# Latest FindBoost.cmake https://raw.githubusercontent.com/Kitware/CMake/master/Modules/FindBoost.cmake
# C:\Dev\CMake\share\cmake-3.7\Modules\FindBoost.cmake
# MSYS:
#   cd /c/dev/libs/boost
#   ./bootstrap.sh gcc --with-libraries=filesystem,system
#   b2 --toolset=gcc link=static address-model=32
# TODO: -x32 is added to the .a and must be removed, line 1474 in .cmake, x64 causes crash with b2 command.
set(BOOST_ROOT "${LIBS_DIR}/boost_1_66_0")
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
target_include_directories(base PUBLIC ${Boost_INCLUDE_DIR})

# TODO: Add FFMPEG
# MSYS:
#   cd /c/dev/libs/ffmpeg
#   ./configure --prefix=build/ --disable-yasm
#   make & make install
# Weird error about 'ERROR_NOT_ENOUGH_MEMORY' undeclared. Try later with other parameters or with earlier version...

# Add SOIL
set(SOIL_ROOT "${LIBS_DIR}/soil")
include_directories(${SOIL_ROOT})
target_include_directories(base PUBLIC ${SOIL_ROOT})
file(GLOB SOIL_SRCS "${SOIL_ROOT}/*.c")
add_library(soil STATIC ${SOIL_SRCS})

# Add GLFW (CMake)
set(GLFW_ROOT "${LIBS_DIR}/glfw-3.2.1")
SET(GLFW_INSTALL ON CACHE BOOL "" FORCE)
include_directories("${GLFW_ROOT}/include")
target_include_directories(base PUBLIC "${GLFW_ROOT}/include")
add_subdirectory("${GLFW_ROOT}" "${CMAKE_CURRENT_BINARY_DIR}/glfw")

# Add Libzip
# Version 1.5.0 gives error about not finding "zipconf.h".
set(LIBZIP_ROOT "${LIBS_DIR}/libzip-1.1.3")
include_directories("${LIBZIP_ROOT}/lib")
target_include_directories(base PUBLIC "${LIBZIP_ROOT}/lib")
file(GLOB LIBZIP_SRCS "${LIBZIP_ROOT}/lib/*.c")
add_library(libzip STATIC ${LIBZIP_SRCS})

# Add FreeType (CMake)
set(FREETYPE_ROOT "${LIBS_DIR}/freetype-2.9")
include_directories("${FREETYPE_ROOT}/include")
target_include_directories(base PUBLIC "${FREETYPE_ROOT}/include")
add_subdirectory(${FREETYPE_ROOT} "${CMAKE_CURRENT_BINARY_DIR}/freetype")

# Add GLEW
# Version 2.1.0 gives warnings about redeclared statements
set(GLEW_ROOT "${LIBS_DIR}/glew-2.0.0")
include_directories("${GLEW_ROOT}/include")
target_include_directories(base PUBLIC "${GLEW_ROOT}/include")
file(GLOB GLEW_SRCS "${GLEW_ROOT}/src/*.c")
add_library(glew STATIC ${GLEW_SRCS})

# Add Zlib
set(ZLIB_ROOT "${LIBS_DIR}/zlib-1.2.11")
include_directories(${ZLIB_ROOT})
target_include_directories(base PUBLIC ${ZLIB_ROOT})
file(GLOB ZLIB_SRCS "${ZLIB_ROOT}/*.c")
add_library(zlib STATIC ${ZLIB_SRCS})

set(LIBS_LINK    soil glfw libzip freetype glew opengl32 zlib ${Boost_LIBRARIES})
set(LIBS_INSTALL soil      libzip          glew          zlib)
set(LIBS_EXPORT  soil glfw libzip freetype glew          zlib)
target_link_libraries(base PUBLIC ${LIBS_LINK})

# https://rix0r.nl/blog/2015/08/13/cmake-guide/
# Define headers for this library. PUBLIC headers are used for
# compiling the library, and will be added to consumers' build
# paths.
target_include_directories(base PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)

# 'make install' to the correct locations (provided by GNUInstallDirs).
include(GNUInstallDirs)
install(TARGETS base ${LIBS_INSTALL} EXPORT baseConfig
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_CURRENT_BINARY_DIR})  # This is for Windows
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# This makes the project importable from the install directory
# Put config file in per-project dir (name MUST match), can also
# just go into 'cmake'.
install(EXPORT baseConfig DESTINATION share/base/cmake)

# This makes the project importable from the build directory
export(TARGETS base ${LIBS_EXPORT} FILE baseConfig.cmake)
