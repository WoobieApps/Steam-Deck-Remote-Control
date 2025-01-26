cmake_minimum_required(VERSION 3.27)

project(SDRC)
set (CMAKE_CXX_STANDARD 20)

# Declare needed dirs
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/inc")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

# Generate the exacutable in /bin dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Fetch dependencies using FetchContent
include(FetchContent)
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL
    GIT_TAG release-2.30.11
)
FetchContent_MakeAvailable(SDL2)

FetchContent_Declare(
    SDL2_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image
    GIT_TAG release-2.8.4
)
FetchContent_MakeAvailable(SDL2_image)

FetchContent_Declare(
    SDL2_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf
    GIT_TAG release-2.24.0
)
FetchContent_MakeAvailable(SDL2_ttf)

# Configure sources in /src directory
add_subdirectory(${SOURCE_DIR})