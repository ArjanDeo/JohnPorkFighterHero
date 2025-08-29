cmake_minimum_required(VERSION 3.10.0)
project(JohnPorkFighterHero VERSION 0.1.0 LANGUAGES C CXX)

find_package(SFML CONFIG REQUIRED COMPONENTS Graphics Window System)

add_executable(JohnPorkFighterHero src/main.cpp)

target_link_libraries(JohnPorkFighterHero PRIVATE SFML::Window SFML::Graphics)