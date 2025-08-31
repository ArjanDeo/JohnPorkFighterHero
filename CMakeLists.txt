cmake_minimum_required(VERSION 3.10.0)
project(JohnPorkFighterHero VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Force MinGW/GCC to statically link its runtime
if (MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
endif()


find_package(SFML CONFIG REQUIRED COMPONENTS Graphics Window System Audio)



add_executable(JohnPorkFighterHero src/main.cpp src/AssetManager.cpp)

add_custom_command(TARGET JohnPorkFighterHero POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:JohnPorkFighterHero>/assets")

target_include_directories(JohnPorkFighterHero PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(JohnPorkFighterHero PRIVATE SFML::Window SFML::Graphics SFML::Audio)