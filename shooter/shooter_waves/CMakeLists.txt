cmake_minimum_required(VERSION 3.28)
project(shooter_waves)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3.0 COMPONENTS Graphics REQUIRED)

add_executable(shooter_waves src/main.cpp)
target_link_libraries(shooter_waves PRIVATE SFML::Graphics)
