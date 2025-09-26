cmake_minimum_required(VERSION 3.10.0)
project(JohnPorkFighterHero VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML CONFIG REQUIRED COMPONENTS Graphics Window System Audio)
find_package(TGUI CONFIG REQUIRED)

add_executable(JohnPorkFighterHero src/Main.cpp src/AssetManager.cpp   "include/MainMenu.h"  "include/MainMenu.h" "src/MainMenu.cpp" "include/Common.h" "src/Common.cpp" "include/SettingsMenu.h" "src/SettingsMenu.cpp" "include/UIElement.h" "src/UI/Button.cpp" "include/UI/Button.h")

add_custom_command(TARGET JohnPorkFighterHero POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:JohnPorkFighterHero>/assets")

target_include_directories(JohnPorkFighterHero PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(JohnPorkFighterHero PRIVATE SFML::Window SFML::Graphics SFML::Audio TGUI::TGUI)

