#include "Block.hpp"

const BlockDef& getBlockDef(BlockID id)
{
    static const BlockDef defs[] =
    {
        { false, false, {0,    0,    0   }, {0,    0,    0   }, {0,    0,    0   } },
        { true,  false, {0.27f,0.60f,0.13f}, {0.42f,0.28f,0.16f}, {0.42f,0.28f,0.16f} },
        { true,  false, {0.42f,0.28f,0.16f}, {0.42f,0.28f,0.16f}, {0.42f,0.28f,0.16f} },
        { true,  false, {0.45f,0.45f,0.45f}, {0.45f,0.45f,0.45f}, {0.45f,0.45f,0.45f} },
        { true,  true,  {0.05f,0.05f,0.05f}, {0.05f,0.05f,0.05f}, {0.05f,0.05f,0.05f} },
    };

    if (id >= 5)
        return defs[0];

    return defs[id];
}