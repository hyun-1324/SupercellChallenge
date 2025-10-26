cmake_minimum_required(VERSION 3.16)
project(Day1HelloSFML)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SFML 3 REQUIRED COMPONENTS Graphics)

add_executable(game src/main.cpp)
target_link_libraries(game PRIVATE SFML::Graphics)