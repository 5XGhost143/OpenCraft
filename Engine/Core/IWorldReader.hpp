#pragma once
#include "Types.hpp"

class IWorldReader
{
public:
    virtual ~IWorldReader() = default;
    virtual BlockID getBlock(int x, int y, int z) const = 0;
    virtual bool    inBounds(int x, int y, int z) const = 0;
};