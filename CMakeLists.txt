cmake_minimum_required(VERSION 3.16)
project(at-project CXX)

# Use C++17 (or later) as required.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# (Optional) Set output directories for binaries and libraries.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT App)

if(MSVC)
    add_compile_options(/utf-8)
endif()
# Add subdirectories for your engine and app.

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(at)
add_subdirectory(app)