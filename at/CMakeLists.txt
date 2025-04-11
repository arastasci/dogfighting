cmake_minimum_required(VERSION 3.16)
project(AtEngine CXX)

# Define the precompiled header file relative to the at folder.
set(PCH_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/atpch.h")

# Gather all source and header files from the engine’s src folder.
file(GLOB_RECURSE ENGINE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE ENGINE_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
file(GLOB_RECURSE STB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb_image/*.cpp")
file(GLOB_RECURSE STB_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb_image/*.h")
# Create the shared library target (DLL on Windows).
add_library(AtEngine SHARED ${ENGINE_SOURCES}  ${ENGINE_HEADERS} ${STB_SOURCES} ${STB_HEADERS})
target_include_directories(AtEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

# AT_DEBUG;AT_CORE;_DEBUG;
add_compile_definitions(AT_DEBUG)
add_compile_definitions(AT_CORE)

# Set up precompiled header (requires CMake 3.16+)
target_precompile_headers(AtEngine PRIVATE ${PCH_FILE})

# Add vendor subdirectories for those that need to be built.
add_subdirectory(vendor/glad)
add_subdirectory(vendor/GLFW)
add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/assimp)

# Include directories for header-only libraries.
target_include_directories(AtEngine PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb_image"
)

# Link the vendor libraries that should be built as static libraries.
target_link_libraries(AtEngine PUBLIC glad glfw)


add_custom_command(TARGET AtEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/res"
        "$<TARGET_FILE_DIR:AtEngine>/res"
    COMMENT "Copying res folder to output directory."
)