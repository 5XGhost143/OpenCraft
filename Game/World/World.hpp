#pragma once
#include "Block.hpp"
#include "../../Engine/Core/IWorldReader.hpp"
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

class World : public IWorldReader
{
public:
    void generate();

    BlockID getBlock(int x, int y, int z) const override;
    void    setBlock(int x, int y, int z, BlockID id);
    bool    inBounds(int x, int y, int z) const override;

    const std::unordered_map<IVec3, BlockID, IVec3Hash>& allBlocks() const;

private:
    std::unordered_map<IVec3, BlockID, IVec3Hash> blocks;
};