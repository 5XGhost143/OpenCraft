#include "HUD.hpp"

void HUD::init()
{
    shader.load("gl_shaders/hud.vert", "gl_shaders/hud.frag");

    glCreateVertexArrays(1, &vao);
    glCreateBuffers(1, &vbo);

    glVertexArrayVertexBuffer(vao, 0, vbo, 0, 5 * sizeof(float));

    glEnableVertexArrayAttrib(vao, 0);
    glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao, 0, 0);

    glEnableVertexArrayAttrib(vao, 1);
    glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(float));
    glVertexArrayAttribBinding(vao, 1, 0);
}

void HUD::drawCrosshair(int w, int h)
{
    float cx    = 0.f;
    float cy    = 0.f;
    float sz    = 0.025f;
    float asp   = (float)w / h;
    float thick = 0.004f;

    float data[] =
    {
        cx - sz / asp, cy - thick,  1, 1, 1,
        cx + sz / asp, cy - thick,  1, 1, 1,
        cx + sz / asp, cy + thick,  1, 1, 1,
        cx - sz / asp, cy - thick,  1, 1, 1,
        cx + sz / asp, cy + thick,  1, 1, 1,
        cx - sz / asp, cy + thick,  1, 1, 1,

        cx - thick / asp, cy - sz,  1, 1, 1,
        cx + thick / asp, cy - sz,  1, 1, 1,
        cx + thick / asp, cy + sz,  1, 1, 1,
        cx - thick / asp, cy - sz,  1, 1, 1,
        cx + thick / asp, cy + sz,  1, 1, 1,
        cx - thick / asp, cy + sz,  1, 1, 1,
    };

    glNamedBufferData(vbo, sizeof(data), data, GL_DYNAMIC_DRAW);
    shader.use();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 12);
}

void HUD::destroy()
{
    shader.destroy();
    if (vao) { glDeleteVertexArrays(1, &vao); vao = 0; }
    if (vbo) { glDeleteBuffers(1, &vbo); vbo = 0; }
}