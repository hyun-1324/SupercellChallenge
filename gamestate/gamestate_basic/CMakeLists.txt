cmake_minimum_required(VERSION 3.28)
project(gamestate_basic)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3.0 COMPONENTS Graphics REQUIRED)

add_executable(gamestate_basic src/main.cpp)
target_link_libraries(gamestate_basic PRIVATE SFML::Graphics)
