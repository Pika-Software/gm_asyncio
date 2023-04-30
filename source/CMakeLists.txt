# Include every source file in source directory
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_LIST_DIR} *.cpp *.hpp)
source_group(TREE ${CMAKE_CURRENT_LIST_DIR} PREFIX "Sources" FILES ${SOURCES})

add_library(async_io SHARED ${SOURCES})
target_link_libraries(async_io PUBLIC 
    gmod::common
    gmod::helpers_extended
    sourcesdk::common
    sourcesdk::tier0
    sourcesdk::tier1
)

target_include_directories(async_io PRIVATE ${CMAKE_CURRENT_LIST_DIR})

set_gmod_suffix_prefix(async_io)

# Autoinstall
set(AUTOINSTALL "" CACHE PATH "Autoinstall path")
if(IS_DIRECTORY ${AUTOINSTALL})
    autoinstall(async_io ${AUTOINSTALL})
endif()
