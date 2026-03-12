#pragma once
#include <GL/glew.h>
#include <string>
#include "../Core/Math.hpp"

class Shader
{
public:
    GLuint id = 0;

    void load(const char* vertPath, const char* fragPath);
    void use() const;
    void destroy();

    void setMat4(const char* name, const Mat4& m) const;
    void setVec3(const char* name, Vec3 v) const;
    void setVec4(const char* name, float x, float y, float z, float w) const;
    void setFloat(const char* name, float v) const;
    void setInt(const char* name, int v) const;

private:
    static std::string read(const char* path);
    static GLuint compile(GLenum type, const std::string& src);
};