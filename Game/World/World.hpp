#pragma once
#include "Block.hpp"
#include <unordered_map>
#include <functional>

constexpr int WORLD_W = 32;
constexpr int WORLD_D = 32;
constexpr int WORLD_H = 256;

constexpr int Y_BEDROCK = 0;
constexpr int Y_STONE_START = 1;
constexpr int Y_STONE_END   = 64;
constexpr int Y_DIRT_START  = 65;
constexpr int Y_DIRT_END    = 71;
constexpr int Y_GRASS       = 72;

struct IVec3Hash {
    size_t operator()(const IVec3& v) const {
        size_t h = (size_t)(v.x * 73856093) ^ (size_t)(v.y * 19349663) ^ (size_t)(v.z * 83492791);
        return h;
    }
};

class World {
public:
    void generate() {
        for (int x=0; x<WORLD_W; ++x)
            for (int z=0; z<WORLD_D; ++z) {
                blocks[{x, Y_BEDROCK, z}] = BLOCK_BEDROCK;
                for (int y=Y_STONE_START; y<=Y_STONE_END; ++y)
                    blocks[{x,y,z}] = BLOCK_STONE;
                for (int y=Y_DIRT_START; y<=Y_DIRT_END; ++y)
                    blocks[{x,y,z}] = BLOCK_DIRT;
                blocks[{x, Y_GRASS, z}] = BLOCK_GRASS;
            }
    }

    BlockID getBlock(int x, int y, int z) const {
        auto it = blocks.find({x,y,z});
        return it != blocks.end() ? it->second : BLOCK_AIR;
    }

    void setBlock(int x, int y, int z, BlockID id) {
        if (id == BLOCK_AIR)
            blocks.erase({x,y,z});
        else
            blocks[{x,y,z}] = id;
    }

    bool inBounds(int x, int y, int z) const {
        return x>=0 && x<WORLD_W && y>=0 && y<WORLD_H && z>=0 && z<WORLD_D;
    }

    const std::unordered_map<IVec3, BlockID, IVec3Hash>& allBlocks() const { return blocks; }

private:
    std::unordered_map<IVec3, BlockID, IVec3Hash> blocks;
};