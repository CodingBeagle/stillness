cmake_minimum_required(VERSION 3.31)
project(stillness)

# The FetchContent module is used to download and manage external dependencies
include(FetchContent)

set(CMAKE_CXX_STANDARD 20)

# Dependencies
set(RAYLIB_VERSION 5.0)

FetchContent_Declare(
        raylib
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
        FIND_PACKAGE_ARGS
)

FetchContent_MakeAvailable(raylib)

# Stillness Project
add_executable(stillness
    main.cpp
    Camera.cpp
    CubeMesh.cpp
    MarchingCubes.cpp
)

# Always copy resources before building the executable
add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:stillness>/resources
    COMMENT "Copying resources to output directory"
)
add_dependencies(stillness copy_resources)

target_link_libraries(stillness PRIVATE raylib)
