#pragma once
#include "../../Engine/Core/Types.hpp"

struct BlockDef
{
    bool  solid;
    bool  unbreakable;
    float colorTop[3];
    float colorSide[3];
    float colorBottom[3];
};

const BlockDef& getBlockDef(BlockID id);