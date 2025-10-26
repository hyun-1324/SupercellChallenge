cmake_minimum_required(VERSION 3.28)
project(sound_music)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3.0 COMPONENTS Graphics Audio REQUIRED)

add_executable(sound_music src/main.cpp)
target_link_libraries(sound_music PRIVATE SFML::Graphics SFML::Audio)
