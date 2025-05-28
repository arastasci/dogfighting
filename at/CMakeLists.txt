cmake_minimum_required(VERSION 3.16)
project(AtEngine CXX)



option(USE_STEAM "Should use Steamworks SDK instead of GameNetworkingSockets" ON)

if(USE_STEAM)
    set(STEAM_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/vendor/steam" CACHE PATH "Steamworks SDK root")
    message("using steam..")
    # find_path(STEAM_INCLUDE_DIR steam/steam_api.h PATHS "${STEAM_ROOT}/public" REQUIRED)
    find_library(STEAM_LIB steam_api64 PATHS "${STEAM_ROOT}/redistributable_bin/win64" REQUIRED)
    find_file(STEAM_DLL steam_api64.dll PATHS "${STEAM_ROOT}/redistributable_bin/win64" REQUIRED)
    add_library(steam_api64 SHARED IMPORTED)
    set_target_properties(steam_api64 PROPERTIES
        IMPORTED_IMPLIB "${STEAM_LIB}"
        IMPORTED_LOCATION "${STEAM_DLL}"
        INTERFACE_INCLUDE_DIRECTORIES "${STEAM_ROOT}/public"
    )
    
else()
    set(GNS_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/vendor/GameNetworkingSockets" CACHE PATH "GameNetworkingSockets Root")
    find_library(GNS_LIB GameNetworkingSockets PATHS "${GNS_ROOT}/bin/Debug" REQUIRED)
    find_file(GNS_DLL GameNetworkingSockets.dll PATHS "${GNS_ROOT}/bin/Debug" REQUIRED)
    add_library(GameNetworkingSockets SHARED IMPORTED)
    set_target_properties(GameNetworkingSockets PROPERTIES
    IMPORTED_IMPLIB "${GNS_LIB}"
    IMPORTED_LOCATION "${GNS_DLL}"
    INTERFACE_INCLUDE_DIRECTORIES "${GNS_ROOT}/include"
    )
    
endif()
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

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Do not build tests" FORCE)
add_subdirectory(vendor/assimp)

add_subdirectory(vendor/bullet)

set_target_properties(glad    PROPERTIES FOLDER "Dependencies/glad")
set_target_properties(spdlog  PROPERTIES FOLDER "Dependencies/spdlog")
set_target_properties(assimp  PROPERTIES FOLDER "Dependencies/assimp")
# set_target_properties(BULLET_PHYSICS  PROPERTIES FOLDER "Dependencies/bullet")

# Organize the engine sources and headers using the folder structure from your file system.
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" 
                FILES ${ENGINE_SOURCES} ${ENGINE_HEADERS})

# Organize the stb_image files if needed.
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb_image" 
                FILES ${STB_SOURCES} ${STB_HEADERS})

# Include directories for header-only libraries.
target_include_directories(AtEngine PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/assimp/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb_image"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/bullet/src"
)


# Link the vendor libraries that should be built as static libraries.
target_link_libraries(AtEngine PUBLIC glad glfw assimp) 

target_link_libraries(AtEngine PUBLIC
    BulletDynamics
    BulletCollision
    LinearMath
    BulletSoftBody
    Bullet3Common
    BulletInverseDynamics
)

if(USE_STEAM)
    target_link_libraries(AtEngine PUBLIC steam_api64)
    target_compile_definitions(AtEngine PUBLIC USE_STEAM)
else()
    target_link_libraries(AtEngine PUBLIC GameNetworkingSockets)
endif()

add_custom_command(TARGET AtEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/res"
        "$<TARGET_FILE_DIR:AtEngine>/res"
    COMMENT "Copying res folder to output directory."
)

if(USE_STEAM)
    add_custom_command(TARGET AtEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_PROPERTY:steam_api64,IMPORTED_LOCATION>"
            "$<TARGET_FILE_DIR:AtEngine>"
    )
else()
    add_custom_command(TARGET AtEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_PROPERTY:GameNetworkingSockets,IMPORTED_LOCATION>"
        "$<TARGET_FILE_DIR:AtEngine>"
    )
endif()