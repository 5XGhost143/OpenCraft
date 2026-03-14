cmake_minimum_required(VERSION 3.16)
project(OpenCraft)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2   CONFIG REQUIRED)
find_package(GLEW   CONFIG REQUIRED)
find_package(OpenGL        REQUIRED)

add_library(5XEngine SHARED
        Engine/Core/Types.cpp
        Engine/Core/Math.cpp
        Engine/Core/Physics.cpp
        Engine/Input/Input.cpp
        Engine/Audio/Audio.cpp
        Engine/Render/Shader.cpp
        Engine/Render/OutlineRenderer.cpp
        Engine/Render/ParticleRenderer.cpp
)

set_target_properties(5XEngine PROPERTIES
        OUTPUT_NAME "5XEngine"
        WINDOWS_EXPORT_ALL_SYMBOLS ON
)

target_include_directories(5XEngine PUBLIC
        Engine/Core
        Engine/Render
        Engine/Input
        Engine/Audio
)

target_link_libraries(5XEngine
        PUBLIC
        SDL2::SDL2
        GLEW::GLEW
        OpenGL::GL
)

add_library(5XGame SHARED
        Game/World/Block.cpp
        Game/World/World.cpp
        Game/World/Particle.cpp
        Game/World/MeshBuilder.cpp
        Game/Player/Camera.cpp
        Game/Player/Raycaster.cpp
        Game/UI/HUD.cpp
)

set_target_properties(5XGame PROPERTIES
        OUTPUT_NAME "5XGame"
        WINDOWS_EXPORT_ALL_SYMBOLS ON
)

target_include_directories(5XGame PUBLIC
        Game/World
        Game/Player
        Game/UI
)

target_link_libraries(5XGame
        PUBLIC
        5XEngine
)

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
        5XEngine
        5XGame
        SDL2::SDL2main
)

if(WIN32)
    add_custom_command(TARGET OpenCraft POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL2::SDL2>
            $<TARGET_FILE_DIR:OpenCraft>
    )

    add_custom_command(TARGET OpenCraft POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:5XEngine>
            $<TARGET_FILE_DIR:OpenCraft>
    )

    add_custom_command(TARGET OpenCraft POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:5XGame>
            $<TARGET_FILE_DIR:OpenCraft>
    )
endif()

add_custom_command(TARGET OpenCraft POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/gl_shaders
        $<TARGET_FILE_DIR:OpenCraft>/gl_shaders
)