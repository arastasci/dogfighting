cmake_minimum_required(VERSION 3.16)
project(SandboxApp CXX)

# Gather the application source files.
file(GLOB_RECURSE APP_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Create the executable target.
add_executable(App ${APP_SOURCES})

# Include the engine’s source directory if needed (for shared headers, etc.).
target_include_directories(App PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../at/src")

# Link against the engine.
target_link_libraries(App PRIVATE AtEngine)
