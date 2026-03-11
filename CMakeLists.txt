cmake_minimum_required(VERSION 4.1)
project(OpenCraft)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2   CONFIG REQUIRED)
find_package(GLEW   CONFIG REQUIRED)
find_package(OpenGL        REQUIRED)

add_executable(OpenCraft OpenCraft.cpp)

target_include_directories(OpenCraft PRIVATE
        Engine/Core
        Engine/Render
        Engine/Input
        Engine/Audio
        Game/World
        Game/Player
        Game/UI
)

target_link_libraries(OpenCraft
        PRIVATE
        SDL2::SDL2
        SDL2::SDL2main
        GLEW::GLEW
        OpenGL::GL
)

if (WIN32)
    add_custom_command(TARGET OpenCraft POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL2::SDL2>
            $<TARGET_FILE_DIR:OpenCraft>
    )
endif()

add_custom_command(TARGET OpenCraft POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/gl_shaders
        $<TARGET_FILE_DIR:OpenCraft>/gl_shaders
)