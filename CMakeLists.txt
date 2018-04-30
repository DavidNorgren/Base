cmake_minimum_required(VERSION 3.7)

# Set C++ settings
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-static-libgcc -static-libstdc++")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
add_definitions("-DEXPORT=__declspec(dllexport)")

# CMake export option
set(BASE_BUILD_TYPE "Debug" CACHE STRING "Base build type")
set_property(CACHE BASE_BUILD_TYPE PROPERTY STRINGS "Debug;Release")
set(CMAKE_BUILD_TYPE ${BASE_BUILD_TYPE})

# Base can be built in any of the following three modes:
# "test" to produce an executable file used for testing
# "shared" to produce a DLL file, used by other applications during development
# "static" to include the code in the final application executable
# BASE_BUILD_STATIC will overwrite this to "static"
if (NOT BASE_BUILD_STATIC)

    set(BASE_BUILD_MODE "Static" CACHE STRING "Base build mode")
    set_property(CACHE BASE_BUILD_MODE PROPERTY STRINGS "Static;Shared;Test")
    option(BASE_DYNAMIC_RESOURCES ON)

else()

    set(BASE_BUILD_MODE "static")
    set(BASE_DYNAMIC_RESOURCES OFF)

endif()

message("[Base] BASE_BUILD_MODE = ${BASE_BUILD_MODE}")

if (BASE_BUILD_MODE STREQUAL "Test")

    project(BaseTest)

    # If dynamic resources are enabled, resources are fetched from an archive during runtime
    if (BASE_DYNAMIC_RESOURCES)

        set(DYNAMIC_RESOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test/res/")

        # These flags enable dynamic loading by the ResourceHandler class
        add_definitions("-DDYNAMIC_RESOURCES")
        add_definitions("-DDYNAMIC_RESOURCES_DIR=\"${DYNAMIC_RESOURCES_DIR}\"")
        message("[Base] Dynamic resources are enabled!")
        message("[Base] DYNAMIC_RESOURCES_DIR = ${DYNAMIC_RESOURCES_DIR}")

    else()

        # Generate object file containing resources on build
        add_custom_command(
            OUTPUT "res.o"
            COMMAND zip -r "${CMAKE_CURRENT_BINARY_DIR}/res.zip" *
            COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}" && ld -r -b binary -o "res.o" "res.zip"
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/test/res"
        )
        set_source_files_properties(
            "res.o"
            PROPERTIES
            EXTERNAL_OBJECT true
            GENERATED true
        )
        set(RES_O "res.o")

    endif()

    # Create executable
    file(GLOB_RECURSE SRCS "*.cpp")
    add_executable(${PROJECT_NAME} ${SRCS} ${RES_O})

    # Clean up
    if (NOT BASE_DYNAMIC_RESOURCES)

        add_custom_command(
            TARGET ${PROJECT_NAME}
            POST_BUILD
            COMMAND rm -f "res.o"
            COMMAND rm -f "res.zip"
            WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        )

    endif()

else()

    project(base)
    file(GLOB_RECURSE SRCS "src/*.cpp")

    # Create library
    if (BASE_BUILD_MODE STREQUAL "Shared")
        add_library(${PROJECT_NAME} SHARED ${SRCS})
    else()
        add_library(${PROJECT_NAME} STATIC ${SRCS})
    endif()

endif()

# Include Base sources
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")

# Add external libraries
# TODO find_package for each!
set(LIBS_DIR "C:/Dev/Libs")

# Add Boost filesystem & system
# Latest FindBoost.cmake https://raw.githubusercontent.com/Kitware/CMake/master/Modules/FindBoost.cmake
# C:\Dev\CMake\share\cmake-3.11\Modules\FindBoost.cmake
# MSYS:
#   cd /c/dev/libs/boost (tab)
#   ./bootstrap.sh gcc --with-libraries=filesystem,system
#   b2 --toolset=gcc link=static address-model=32
# TODO: -x32 is added to the .a and must be removed, line 1474 in .cmake, x64 causes crash with b2 command.
set(BOOST_ROOT "${LIBS_DIR}/boost_1_67_0")
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIR})

# TODO: Add FFMPEG
# MSYS:
#   cd /c/dev/libs/ffmpeg
#   ./configure --prefix=build/ --disable-yasm
#   make & make install
# Weird error about 'ERROR_NOT_ENOUGH_MEMORY' undeclared. Try later with other parameters or with earlier version...

# Add SOIL
set(SOIL_ROOT "${LIBS_DIR}/soil")
include_directories(${SOIL_ROOT})
target_include_directories(${PROJECT_NAME} PUBLIC ${SOIL_ROOT})
file(GLOB SOIL_SRCS "${SOIL_ROOT}/*.c")
add_library(soil STATIC ${SOIL_SRCS})

# Add tinyobjloader
set(TINYOBJLOADER_ROOT "${LIBS_DIR}/tinyobjloader-1.1.1")
include_directories(${TINYOBJLOADER_ROOT})
target_include_directories(${PROJECT_NAME} PUBLIC ${TINYOBJLOADER_ROOT})

# Add GLFW (CMake)
set(GLFW_ROOT "${LIBS_DIR}/glfw-3.2.1")
SET(GLFW_INSTALL ON CACHE BOOL "" FORCE)
include_directories("${GLFW_ROOT}/include")
target_include_directories(${PROJECT_NAME} PUBLIC "${GLFW_ROOT}/include")
add_subdirectory(${GLFW_ROOT} "${CMAKE_CURRENT_BINARY_DIR}/glfw")

# Add Libzip
# Version 1.5.0 gives error about not finding "zipconf.h".
set(LIBZIP_ROOT "${LIBS_DIR}/libzip-1.1.3")
include_directories("${LIBZIP_ROOT}/lib")
target_include_directories(${PROJECT_NAME} PUBLIC "${LIBZIP_ROOT}/lib")
file(GLOB LIBZIP_SRCS "${LIBZIP_ROOT}/lib/*.c")
add_library(libzip STATIC ${LIBZIP_SRCS})

# Add FreeType (CMake)
set(FREETYPE_ROOT "${LIBS_DIR}/freetype-2.9")
include_directories("${FREETYPE_ROOT}/include")
target_include_directories(${PROJECT_NAME} PUBLIC "${FREETYPE_ROOT}/include")
add_subdirectory(${FREETYPE_ROOT} "${CMAKE_CURRENT_BINARY_DIR}/freetype")

# Add GLEW
# Version 2.1.0 gives warnings about redeclared statements
set(GLEW_ROOT "${LIBS_DIR}/glew-2.0.0")
include_directories("${GLEW_ROOT}/include")
target_include_directories(${PROJECT_NAME} PUBLIC "${GLEW_ROOT}/include")
file(GLOB GLEW_SRCS "${GLEW_ROOT}/src/*.c")
add_library(glew STATIC ${GLEW_SRCS})

# Add Zlib
set(ZLIB_ROOT "${LIBS_DIR}/zlib-1.2.11")
include_directories(${ZLIB_ROOT})
target_include_directories(${PROJECT_NAME} PUBLIC ${ZLIB_ROOT})
file(GLOB ZLIB_SRCS "${ZLIB_ROOT}/*.c")
add_library(zlib STATIC ${ZLIB_SRCS})

set(LIBS_LINK    soil glfw libzip freetype glew opengl32 zlib ${Boost_LIBRARIES})
set(LIBS_INSTALL soil      libzip          glew          zlib)
set(LIBS_EXPORT  soil glfw libzip freetype glew          zlib)
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBS_LINK})

if (BASE_BUILD_MODE STREQUAL "Shared")

    # https://rix0r.nl/blog/2015/08/13/cmake-guide/
    # Define headers for this library. PUBLIC headers are used for
    # compiling the library, and will be added to consumers' build
    # paths.
    target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
        PRIVATE src)

    # 'make install' to the correct locations (provided by GNUInstallDirs).
    include(GNUInstallDirs)
    install(TARGETS ${PROJECT_NAME} ${LIBS_INSTALL} EXPORT baseConfig
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_CURRENT_BINARY_DIR})  # This is for Windows
    install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

    # This makes the project importable from the install directory
    # Put config file in per-project dir (name MUST match), can also
    # just go into 'cmake'.
    install(EXPORT baseConfig DESTINATION share/base/cmake)

    # This makes the project importable from the build directory
    export(TARGETS ${PROJECT_NAME} ${LIBS_EXPORT} FILE baseConfig.cmake)

endif()