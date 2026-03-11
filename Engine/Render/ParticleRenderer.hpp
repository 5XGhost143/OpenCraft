#pragma once
#include "Shader.hpp"
#include "../../Game/World/Particle.hpp"
#include <GL/glew.h>
#include <vector>

class ParticleRenderer {
public:
    Shader shader;
    GLuint vao=0, vbo=0;
    int maxParticles = 2048;

    void init() {
        shader.load("gl_shaders/particle.vert", "gl_shaders/particle.frag");
        glCreateVertexArrays(1, &vao);
        glCreateBuffers(1, &vbo);

        int stride = 8 * sizeof(float);
        glNamedBufferData(vbo, maxParticles * stride, nullptr, GL_DYNAMIC_DRAW);
        glVertexArrayVertexBuffer(vao, 0, vbo, 0, stride);

        glEnableVertexArrayAttrib(vao, 0);
        glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribBinding(vao, 0, 0);

        glEnableVertexArrayAttrib(vao, 1);
        glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float));
        glVertexArrayAttribBinding(vao, 1, 0);

        glEnableVertexArrayAttrib(vao, 2);
        glVertexArrayAttribFormat(vao, 2, 1, GL_FLOAT, GL_FALSE, 6*sizeof(float));
        glVertexArrayAttribBinding(vao, 2, 0);

        glEnableVertexArrayAttrib(vao, 3);
        glVertexArrayAttribFormat(vao, 3, 1, GL_FLOAT, GL_FALSE, 7*sizeof(float));
        glVertexArrayAttribBinding(vao, 3, 0);
    }

    void draw(const ParticleSystem& ps, const Mat4& vp) {
        if (ps.particles.empty()) return;
        std::vector<float> data;
        data.reserve(ps.particles.size() * 8);
        for (auto& p : ps.particles) {
            float lifeRatio = p.life / p.maxLife;
            data.push_back(p.pos.x); data.push_back(p.pos.y); data.push_back(p.pos.z);
            data.push_back(p.r); data.push_back(p.g); data.push_back(p.b);
            data.push_back(lifeRatio);
            data.push_back(p.size);
        }
        int count = (int)ps.particles.size();
        glNamedBufferSubData(vbo, 0, count * 8 * sizeof(float), data.data());

        shader.use();
        shader.setMat4("u_vp", vp);
        glBindVertexArray(vao);
        glEnable(GL_PROGRAM_POINT_SIZE);
        glDepthMask(GL_FALSE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawArrays(GL_POINTS, 0, count);
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
    }

    void destroy() {
        shader.destroy();
        if (vao) { glDeleteVertexArrays(1,&vao); vao=0; }
        if (vbo) { glDeleteBuffers(1,&vbo); vbo=0; }
    }
};