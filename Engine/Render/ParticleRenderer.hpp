#pragma once
#include "Shader.hpp"
#include <GL/glew.h>

class ParticleSystem;

class ParticleRenderer
{
public:
    Shader shader;
    GLuint vao          = 0;
    GLuint vbo          = 0;
    int    maxParticles = 2048;

    void init();
    void draw(const ParticleSystem& ps, const Mat4& vp);
    void destroy();
};