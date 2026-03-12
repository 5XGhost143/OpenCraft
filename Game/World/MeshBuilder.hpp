#pragma once
#include <GL/glew.h>

class World;

struct Vertex
{
    float x, y, z;
    float r, g, b;
    float ao;
};

class WorldMesh
{
public:
    GLuint vao            = 0;
    GLuint vbo            = 0;
    int    vertCount      = 0;
    bool   dirty          = true;
    int    allocatedVerts = 0;

    void init();
    void build(const World& world);
    void draw() const;
    void destroy();
};