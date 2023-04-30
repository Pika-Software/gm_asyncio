cmake_minimum_required(VERSION 3.22)

# Require C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable -fPIC flag
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Enable IDE folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Force x86_64 architecture on MacOSX
if(APPLE)
    set(CMAKE_OSX_ARCHITECTURES "x86_64")

    # Disable deprecation warnings
    add_compile_options(
        -Wno-deprecated-declarations
    )
endif()

# Enable multithreaded compilation on Windows
if(MSVC)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" "/MP")
endif()

# Include extensions
add_subdirectory(cmake)

# Include garrysmod_common
find_garrysmod_common()
if(NOT GARRYSMOD_COMMON_FOUND) # Check if garrysmod_common has been found
    message(FATAL_ERROR "garrysmod_common not found")
endif()

project(gm_asyncio VERSION 1.0.0 LANGUAGES CXX)

add_subdirectory(source)
