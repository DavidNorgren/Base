CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set project
SET(PROJECTNAME Base)
SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS "-g")
SET(CMAKE_CXX_FLAGS_DEBUG "-g")
SET(LIBS_DIR "C:/Dev/Libs")
PROJECT(${PROJECTNAME})

# Generate object file containing resources on build
ADD_CUSTOM_COMMAND(
    OUTPUT "res.o"
    COMMAND zip -r "${CMAKE_CURRENT_BINARY_DIR}/res.zip" *
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}" && ld -r -b binary -o "res.o" "res.zip"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/res"
)
SET_SOURCE_FILES_PROPERTIES(
    "res.o"
    PROPERTIES
    EXTERNAL_OBJECT true
    GENERATED true
)

# Command for cleaning old resources, run as "make clean_res"
ADD_CUSTOM_TARGET(clean_res)
ADD_CUSTOM_COMMAND(
    TARGET clean_res
    COMMAND rm -f "${CMAKE_CURRENT_BINARY_DIR}/res.zip"
    COMMAND rm -f "${CMAKE_CURRENT_BINARY_DIR}/res.o"
    COMMAND echo "Cleaned old resources!"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)

# Add Base sources
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/src")
FILE(GLOB_RECURSE BASE_SRCS "src/*.cpp")
FILE(GLOB_RECURSE BASE_HDRS "src/*.hpp")
ADD_LIBRARY(${PROJECTNAME} STATIC ${BASE_SRCS} ${BASE_HDRS} "res.o")

# Add Boost filesystem & system
# Latest FindBoost.cmake https://raw.githubusercontent.com/Kitware/CMake/master/Modules/FindBoost.cmake
# C:\Dev\CMake\share\cmake-3.7\Modules\FindBoost.cmake
# MSYS:
#   cd /c/dev/libs/boost
#   ./bootstrap.sh gcc --with-libraries=filesystem,system
#   b2 --toolset=gcc link=static address-model=32
# TODO: -x32 is added to the .a and must be removed, line 1474 in .cmake, x64 causes crash with b2 command.
SET(BOOST_ROOT "${LIBS_DIR}/boost_1_66_0")
SET(Boost_USE_STATIC_LIBS ON)
FIND_PACKAGE(Boost COMPONENTS system filesystem REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
TARGET_INCLUDE_DIRECTORIES(${PROJECTNAME} PUBLIC ${Boost_INCLUDE_DIR})

# TODO: Add FFMPEG
# MSYS:
#   cd /c/dev/libs/ffmpeg
#   ./configure --prefix=build/ --disable-yasm
#   make & make install
# Weird error about 'ERROR_NOT_ENOUGH_MEMORY' undeclared. Try later with other parameters or with earlier version...

# Add SOIL
SET(SOIL_ROOT "${LIBS_DIR}/soil")
INCLUDE_DIRECTORIES(${SOIL_ROOT})
TARGET_INCLUDE_DIRECTORIES(${PROJECTNAME} PUBLIC ${SOIL_ROOT})
FILE(GLOB SOIL_SRCS "${SOIL_ROOT}/*.c")
ADD_LIBRARY(soil STATIC ${SOIL_SRCS})

# Add GLFW (CMake)
SET(GLFW_ROOT "${LIBS_DIR}/glfw-3.2.1")
INCLUDE_DIRECTORIES("${GLFW_ROOT}/include")
TARGET_INCLUDE_DIRECTORIES(${PROJECTNAME} PUBLIC "${GLFW_ROOT}/include")
ADD_SUBDIRECTORY("${GLFW_ROOT}" "${CMAKE_CURRENT_BINARY_DIR}/glfw")

# Add Libzip
# Version 1.5.0 gives error about not finding "zipconf.h".
SET(LIBZIP_ROOT "${LIBS_DIR}/libzip-1.1.3")
INCLUDE_DIRECTORIES("${LIBZIP_ROOT}/lib")
TARGET_INCLUDE_DIRECTORIES(${PROJECTNAME} PUBLIC "${LIBZIP_ROOT}/lib")
FILE(GLOB LIBZIP_SRCS "${LIBZIP_ROOT}/lib/*.c")
ADD_LIBRARY(libzip STATIC ${LIBZIP_SRCS})

# Add FreeType (CMake)
SET(FREETYPE_ROOT "${LIBS_DIR}/freetype-2.9")
INCLUDE_DIRECTORIES("${FREETYPE_ROOT}/include")
TARGET_INCLUDE_DIRECTORIES(${PROJECTNAME} PUBLIC "${FREETYPE_ROOT}/include")
ADD_SUBDIRECTORY(${FREETYPE_ROOT} "${CMAKE_CURRENT_BINARY_DIR}/freetype")

# Add GLEW
# Version 2.1.0 gives warnings about redeclared statements
SET(GLEW_ROOT "${LIBS_DIR}/glew-2.0.0")
INCLUDE_DIRECTORIES("${GLEW_ROOT}/include")
TARGET_INCLUDE_DIRECTORIES(${PROJECTNAME} PUBLIC "${GLEW_ROOT}/include")
FILE(GLOB GLEW_SRCS "${GLEW_ROOT}/src/*.c")
ADD_LIBRARY(glew STATIC ${GLEW_SRCS})

# Add Zlib
SET(ZLIB_ROOT "${LIBS_DIR}/zlib-1.2.11")
INCLUDE_DIRECTORIES(${ZLIB_ROOT})
TARGET_INCLUDE_DIRECTORIES(${PROJECTNAME} PUBLIC ${ZLIB_ROOT})
FILE(GLOB ZLIB_SRCS "${ZLIB_ROOT}/*.c")
ADD_LIBRARY(zlib STATIC ${ZLIB_SRCS})

TARGET_LINK_LIBRARIES(${PROJECTNAME} ${Boost_LIBRARIES} soil glfw libzip freetype glew opengl32 zlib)