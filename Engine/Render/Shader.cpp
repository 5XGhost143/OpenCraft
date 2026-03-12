#include "Shader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

void Shader::load(const char* vertPath, const char* fragPath)
{
    GLuint vert = compile(GL_VERTEX_SHADER,   read(vertPath));
    GLuint frag = compile(GL_FRAGMENT_SHADER, read(fragPath));
    id = glCreateProgram();
    glAttachShader(id, vert);
    glAttachShader(id, frag);
    glLinkProgram(id);
    GLint ok = 0;
    glGetProgramiv(id, GL_LINK_STATUS, &ok);
    if (!ok)
    {
        char log[1024];
        glGetProgramInfoLog(id, sizeof(log), nullptr, log);
        std::cerr << log << '\n';
    }
    glDeleteShader(vert);
    glDeleteShader(frag);
}

void Shader::use() const
{
    glUseProgram(id);
}

void Shader::destroy()
{
    if (id)
    {
        glDeleteProgram(id);
        id = 0;
    }
}

void Shader::setMat4(const char* name, const Mat4& m) const
{
    glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, m.m);
}

void Shader::setVec3(const char* name, Vec3 v) const
{
    glUniform3f(glGetUniformLocation(id, name), v.x, v.y, v.z);
}

void Shader::setVec4(const char* name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(id, name), x, y, z, w);
}

void Shader::setFloat(const char* name, float v) const
{
    glUniform1f(glGetUniformLocation(id, name), v);
}

void Shader::setInt(const char* name, int v) const
{
    glUniform1i(glGetUniformLocation(id, name), v);
}

std::string Shader::read(const char* path)
{
    std::ifstream f(path);
    if (!f)
    {
        std::cerr << "Cannot open: " << path << '\n';
        return {};
    }
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

GLuint Shader::compile(GLenum type, const std::string& src)
{
    const char* ptr = src.c_str();
    GLuint sid = glCreateShader(type);
    glShaderSource(sid, 1, &ptr, nullptr);
    glCompileShader(sid);
    GLint ok = 0;
    glGetShaderiv(sid, GL_COMPILE_STATUS, &ok);
    if (!ok)
    {
        char log[1024];
        glGetShaderInfoLog(sid, sizeof(log), nullptr, log);
        std::cerr << log << '\n';
    }
    return sid;
}