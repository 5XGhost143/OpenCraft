#pragma once
#include "Shader.hpp"
#include <GL/glew.h>

class OutlineRenderer
{
public:
    Shader shader;
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;

    void init();
    void draw(Vec3 blockPos, const Mat4& vp, float breakAnim, float time);
    void destroy();
};