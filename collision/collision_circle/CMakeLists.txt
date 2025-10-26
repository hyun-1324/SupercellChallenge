cmake_minimum_required(VERSION 3.28)
project(collision_circle)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3.0 COMPONENTS Graphics REQUIRED)

add_executable(game src/main.cpp)
target_link_libraries(game PRIVATE SFML::Graphics)
