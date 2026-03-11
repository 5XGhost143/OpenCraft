#pragma once
#include "Shader.hpp"
#include <GL/glew.h>

class OutlineRenderer {
public:
    Shader shader;
    GLuint vao=0, vbo=0, ebo=0;

    void init() {
        shader.load("gl_shaders/outline.vert", "gl_shaders/outline.frag");

        float verts[] = {
            0,0,0, 1,0,0, 1,1,0, 0,1,0,
            0,0,1, 1,0,1, 1,1,1, 0,1,1,
        };
        uint8_t idx[] = {
            0,1, 1,2, 2,3, 3,0,
            4,5, 5,6, 6,7, 7,4,
            0,4, 1,5, 2,6, 3,7,
        };

        glCreateVertexArrays(1, &vao);
        glCreateBuffers(1, &vbo);
        glCreateBuffers(1, &ebo);

        glNamedBufferStorage(vbo, sizeof(verts), verts, 0);
        glNamedBufferStorage(ebo, sizeof(idx), idx, 0);

        glVertexArrayVertexBuffer(vao, 0, vbo, 0, 3*sizeof(float));
        glVertexArrayElementBuffer(vao, ebo);

        glEnableVertexArrayAttrib(vao, 0);
        glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribBinding(vao, 0, 0);
    }

    void draw(Vec3 blockPos, const Mat4& vp, float breakAnim, float time) {
        shader.use();
        shader.setMat4("u_vp", vp);
        shader.setVec3("u_blockPos", blockPos);
        shader.setFloat("u_breakAnim", breakAnim);
        shader.setFloat("u_time", time);
        glBindVertexArray(vao);
        glLineWidth(2.0f);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthFunc(GL_LEQUAL);
        glDrawElements(GL_LINES, 24, GL_UNSIGNED_BYTE, nullptr);
        glDepthFunc(GL_LESS);
        glDisable(GL_BLEND);
    }

    void destroy() {
        shader.destroy();
        if (vao) { glDeleteVertexArrays(1,&vao); vao=0; }
        if (vbo) { glDeleteBuffers(1,&vbo); vbo=0; }
        if (ebo) { glDeleteBuffers(1,&ebo); ebo=0; }
    }
};