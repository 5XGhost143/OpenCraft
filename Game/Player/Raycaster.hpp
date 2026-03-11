#pragma once
#include "../../Engine/Core/Types.hpp"
#include "../World/World.hpp"
#include <cmath>

struct RayHit {
    bool hit;
    IVec3 block;
    IVec3 normal;
    float dist;
};

inline RayHit raycast(const World& world, Vec3 origin, Vec3 dir, float maxDist = 8.f) {
    float dx = dir.x, dy = dir.y, dz = dir.z;

    int ix = (int)std::floor(origin.x);
    int iy = (int)std::floor(origin.y);
    int iz = (int)std::floor(origin.z);

    int stepX = dx > 0 ? 1 : -1;
    int stepY = dy > 0 ? 1 : -1;
    int stepZ = dz > 0 ? 1 : -1;

    float tDeltaX = dx != 0 ? std::abs(1.f/dx) : 1e30f;
    float tDeltaY = dy != 0 ? std::abs(1.f/dy) : 1e30f;
    float tDeltaZ = dz != 0 ? std::abs(1.f/dz) : 1e30f;

    float tMaxX = dx != 0 ? ((dx>0 ? (ix+1-origin.x) : (origin.x-ix)) / std::abs(dx)) : 1e30f;
    float tMaxY = dy != 0 ? ((dy>0 ? (iy+1-origin.y) : (origin.y-iy)) / std::abs(dy)) : 1e30f;
    float tMaxZ = dz != 0 ? ((dz>0 ? (iz+1-origin.z) : (origin.z-iz)) / std::abs(dz)) : 1e30f;

    IVec3 normal = {0, 0, 0};
    float t = 0.f;

    for (int i = 0; i < 256; ++i) {
        BlockID b = world.getBlock(ix, iy, iz);
        if (b != BLOCK_AIR && i > 0) {
            return {true, {ix, iy, iz}, normal, t};
        }

        if (tMaxX < tMaxY && tMaxX < tMaxZ) {
            if (tMaxX > maxDist) break;
            t = tMaxX;
            normal = {-stepX, 0, 0};
            ix += stepX;
            tMaxX += tDeltaX;
        } else if (tMaxY < tMaxZ) {
            if (tMaxY > maxDist) break;
            t = tMaxY;
            normal = {0, -stepY, 0};
            iy += stepY;
            tMaxY += tDeltaY;
        } else {
            if (tMaxZ > maxDist) break;
            t = tMaxZ;
            normal = {0, 0, -stepZ};
            iz += stepZ;
            tMaxZ += tDeltaZ;
        }
    }
    return {false, {0,0,0}, {0,0,0}, 0};
}