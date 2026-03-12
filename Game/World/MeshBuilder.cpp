#include "MeshBuilder.hpp"
#include "World.hpp"
#include "Block.hpp"
#include <vector>

void WorldMesh::init()
{
    glCreateVertexArrays(1, &vao);
    glCreateBuffers(1, &vbo);

    glEnableVertexArrayAttrib(vao, 0);
    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, x));
    glVertexArrayAttribBinding(vao, 0, 0);

    glEnableVertexArrayAttrib(vao, 1);
    glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, r));
    glVertexArrayAttribBinding(vao, 1, 0);

    glEnableVertexArrayAttrib(vao, 2);
    glVertexArrayAttribFormat(vao, 2, 1, GL_FLOAT, GL_FALSE, offsetof(Vertex, ao));
    glVertexArrayAttribBinding(vao, 2, 0);
}

void WorldMesh::build(const World& world)
{
    std::vector<Vertex> verts;
    verts.reserve(65536);

    auto& all = world.allBlocks();

    for (auto& [pos, id] : all)
    {
        if (id == BLOCK_AIR)
            continue;

        const BlockDef& def = getBlockDef(id);
        int x = pos.x, y = pos.y, z = pos.z;

        auto quad = [&](
            float ax, float ay, float az,
            float bx, float by, float bz,
            float cx, float cy, float cz,
            float dx, float dy, float dz,
            const float* col, float shade)
        {
            float r = col[0] * shade;
            float g = col[1] * shade;
            float b = col[2] * shade;
            verts.push_back({ax, ay, az, r, g, b, 1});
            verts.push_back({bx, by, bz, r, g, b, 1});
            verts.push_back({cx, cy, cz, r, g, b, 1});
            verts.push_back({ax, ay, az, r, g, b, 1});
            verts.push_back({cx, cy, cz, r, g, b, 1});
            verts.push_back({dx, dy, dz, r, g, b, 1});
        };

        float x0 = x, x1 = x + 1;
        float y0 = y, y1 = y + 1;
        float z0 = z, z1 = z + 1;

        if (world.getBlock(x, y + 1, z) == BLOCK_AIR)
            quad(x0,y1,z1, x1,y1,z1, x1,y1,z0, x0,y1,z0, def.colorTop, 1.0f);

        if (world.getBlock(x, y - 1, z) == BLOCK_AIR)
            quad(x0,y0,z0, x1,y0,z0, x1,y0,z1, x0,y0,z1, def.colorBottom, 0.5f);

        if (world.getBlock(x + 1, y, z) == BLOCK_AIR)
            quad(x1,y0,z0, x1,y1,z0, x1,y1,z1, x1,y0,z1, def.colorSide, 0.8f);

        if (world.getBlock(x - 1, y, z) == BLOCK_AIR)
            quad(x0,y0,z1, x0,y1,z1, x0,y1,z0, x0,y0,z0, def.colorSide, 0.8f);

        if (world.getBlock(x, y, z + 1) == BLOCK_AIR)
            quad(x1,y0,z1, x1,y1,z1, x0,y1,z1, x0,y0,z1, def.colorSide, 0.9f);

        if (world.getBlock(x, y, z - 1) == BLOCK_AIR)
            quad(x0,y0,z0, x0,y1,z0, x1,y1,z0, x1,y0,z0, def.colorSide, 0.9f);
    }

    vertCount = (int)verts.size();

    if (vertCount > 0)
    {
        if (vertCount > allocatedVerts)
        {
            int newAlloc = vertCount * 2;
            glNamedBufferData(vbo, newAlloc * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
            allocatedVerts = newAlloc;
            glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vertex));
        }
        glNamedBufferSubData(vbo, 0, vertCount * sizeof(Vertex), verts.data());
    }

    dirty = false;
}

void WorldMesh::draw() const
{
    if (vertCount == 0)
        return;

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertCount);
}

void WorldMesh::destroy()
{
    if (vao) { glDeleteVertexArrays(1, &vao); vao = 0; }
    if (vbo) { glDeleteBuffers(1, &vbo); vbo = 0; }
}