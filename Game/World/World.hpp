#pragma once
#include "Block.hpp"
#include <unordered_map>

constexpr int WORLD_W = 32;
constexpr int WORLD_D = 32;
constexpr int WORLD_H = 256;

constexpr int Y_BEDROCK    = 0;
constexpr int Y_STONE_START = 1;
constexpr int Y_STONE_END   = 64;
constexpr int Y_DIRT_START  = 65;
constexpr int Y_DIRT_END    = 71;
constexpr int Y_GRASS       = 72;

struct IVec3Hash
{
    size_t operator()(const IVec3& v) const;
};

class World
{
public:
    void generate();

    BlockID getBlock(int x, int y, int z) const;
    void    setBlock(int x, int y, int z, BlockID id);
    bool    inBounds(int x, int y, int z) const;

    const std::unordered_map<IVec3, BlockID, IVec3Hash>& allBlocks() const;

private:
    std::unordered_map<IVec3, BlockID, IVec3Hash> blocks;
};