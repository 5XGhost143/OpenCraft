#pragma once
#include "../../Engine/Render/Shader.hpp"
#include <GL/glew.h>

class HUD
{
public:
    Shader shader;
    GLuint vao = 0;
    GLuint vbo = 0;

    void init();
    void drawCrosshair(int w, int h);
    void destroy();
};