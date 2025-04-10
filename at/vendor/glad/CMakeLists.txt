cmake_minimum_required(VERSION 3.16)
project(glad C)

# Find all glad sources (here we assume there is a single source file).
file(GLOB GLAD_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c")

# Create a static library target for glad.
add_library(glad STATIC ${GLAD_SRC})
target_include_directories(glad PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
