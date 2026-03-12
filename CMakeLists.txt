cmake_minimum_required(VERSION 3.16)
project(OpenCraft)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2   CONFIG REQUIRED)
find_package(GLEW   CONFIG REQUIRED)
find_package(OpenGL        REQUIRED)

set(SOURCES
        OpenCraft.cpp

        Engine/Core/Types.cpp
        Engine/Core/Math.cpp
        Engine/Core/Physics.cpp
        Engine/Input/Input.cpp
        Engine/Audio/Audio.cpp
        Engine/Render/Shader.cpp
        Engine/Render/OutlineRenderer.cpp
        Engine/Render/ParticleRenderer.cpp

        Game/World/Block.cpp
        Game/World/World.cpp
        Game/World/Particle.cpp
        Game/World/MeshBuilder.cpp
        Game/Player/Camera.cpp
        Game/Player/Raycaster.cpp
        Game/UI/HUD.cpp
)

add_executable(OpenCraft ${SOURCES})

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

if(WIN32)
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