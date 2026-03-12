#pragma once
#include "../../Engine/Core/Types.hpp"

class World;

struct RayHit
{
    bool  hit;
    IVec3 block;
    IVec3 normal;
    float dist;
};

RayHit raycast(const World& world, Vec3 origin, Vec3 dir, float maxDist = 8.f);