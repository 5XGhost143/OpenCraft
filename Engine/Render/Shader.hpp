#pragma once
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "../Core/Math.hpp"

class Shader {
public:
    GLuint id = 0;

    void load(const char* vertPath, const char* fragPath) {
        GLuint vert = compile(GL_VERTEX_SHADER,   read(vertPath));
        GLuint frag = compile(GL_FRAGMENT_SHADER, read(fragPath));
        id = glCreateProgram();
        glAttachShader(id, vert);
        glAttachShader(id, frag);
        glLinkProgram(id);
        GLint ok=0; glGetProgramiv(id, GL_LINK_STATUS, &ok);
        if (!ok) { char log[1024]; glGetProgramInfoLog(id,sizeof(log),nullptr,log); std::cerr<<log<<'\n'; }
        glDeleteShader(vert);
        glDeleteShader(frag);
    }

    void use() const { glUseProgram(id); }

    void setMat4(const char* name, const Mat4& m) const {
        glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, m.m);
    }
    void setVec3(const char* name, Vec3 v) const {
        glUniform3f(glGetUniformLocation(id, name), v.x, v.y, v.z);
    }
    void setVec4(const char* name, float x, float y, float z, float w) const {
        glUniform4f(glGetUniformLocation(id, name), x, y, z, w);
    }
    void setFloat(const char* name, float v) const {
        glUniform1f(glGetUniformLocation(id, name), v);
    }
    void setInt(const char* name, int v) const {
        glUniform1i(glGetUniformLocation(id, name), v);
    }

    void destroy() { if (id) { glDeleteProgram(id); id=0; } }

private:
    static std::string read(const char* path) {
        std::ifstream f(path);
        if (!f) { std::cerr<<"Cannot open: "<<path<<'\n'; return {}; }
        std::ostringstream ss; ss << f.rdbuf(); return ss.str();
    }
    static GLuint compile(GLenum type, const std::string& src) {
        const char* ptr = src.c_str();
        GLuint id = glCreateShader(type);
        glShaderSource(id, 1, &ptr, nullptr);
        glCompileShader(id);
        GLint ok=0; glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
        if (!ok) { char log[1024]; glGetShaderInfoLog(id,sizeof(log),nullptr,log); std::cerr<<log<<'\n'; }
        return id;
    }
};