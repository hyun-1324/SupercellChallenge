cmake_minimum_required(VERSION 3.22.1)
project(RotatingHammer LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# SFML 찾기
find_package(SFML 2.5 COMPONENTS graphics window system QUIET)

if(NOT SFML_FOUND)
    include(FetchContent)
    FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x
        GIT_SHALLOW ON)
    FetchContent_MakeAvailable(SFML)
endif()

file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/*.h
)

add_executable(rotating_hammer ${SOURCES})
target_link_libraries(rotating_hammer PRIVATE sfml-graphics sfml-window sfml-system)
target_compile_features(rotating_hammer PRIVATE cxx_std_17)
