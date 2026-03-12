#include "World.hpp"

size_t IVec3Hash::operator()(const IVec3& v) const
{
    size_t h = (size_t)(v.x * 73856093) ^ (size_t)(v.y * 19349663) ^ (size_t)(v.z * 83492791);
    return h;
}

void World::generate()
{
    for (int x = 0; x < WORLD_W; ++x)
    {
        for (int z = 0; z < WORLD_D; ++z)
        {
            blocks[{x, Y_BEDROCK, z}] = BLOCK_BEDROCK;

            for (int y = Y_STONE_START; y <= Y_STONE_END; ++y)
                blocks[{x, y, z}] = BLOCK_STONE;

            for (int y = Y_DIRT_START; y <= Y_DIRT_END; ++y)
                blocks[{x, y, z}] = BLOCK_DIRT;

            blocks[{x, Y_GRASS, z}] = BLOCK_GRASS;
        }
    }
}

BlockID World::getBlock(int x, int y, int z) const
{
    auto it = blocks.find({x, y, z});
    return it != blocks.end() ? it->second : BLOCK_AIR;
}

void World::setBlock(int x, int y, int z, BlockID id)
{
    if (id == BLOCK_AIR)
        blocks.erase({x, y, z});
    else
        blocks[{x, y, z}] = id;
}

bool World::inBounds(int x, int y, int z) const
{
    return x >= 0 && x < WORLD_W && y >= 0 && y < WORLD_H && z >= 0 && z < WORLD_D;
}

const std::unordered_map<IVec3, BlockID, IVec3Hash>& World::allBlocks() const
{
    return blocks;
}