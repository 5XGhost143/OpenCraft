#include "Physics.hpp"
#include "../../Game/World/World.hpp"
#include <cmath>

Vec3 Player::eyePos() const
{
    return {pos.x, pos.y + EYE_HEIGHT, pos.z};
}

bool Player::collidesAt(const World& world, Vec3 p) const
{
    int x0 = (int)std::floor(p.x - HALF_W);
    int x1 = (int)std::floor(p.x + HALF_W - 0.001f);
    int y0 = (int)std::floor(p.y);
    int y1 = (int)std::floor(p.y + PLAYER_HEIGHT - 0.001f);
    int z0 = (int)std::floor(p.z - HALF_W);
    int z1 = (int)std::floor(p.z + HALF_W - 0.001f);

    for (int bx = x0; bx <= x1; ++bx)
        for (int by = y0; by <= y1; ++by)
            for (int bz = z0; bz <= z1; ++bz)
                if (world.getBlock(bx, by, bz) != BLOCK_AIR)
                    return true;

    return false;
}

void Player::update(float dt, const World& world, Vec3 wishMove, bool jumpPressed)
{
    vel.y -= GRAVITY * dt;

    if (onGround && jumpPressed)
        vel.y = JUMP_SPEED;

    Vec3 move = wishMove * MOVE_SPEED;
    vel.x = move.x;
    vel.z = move.z;

    Vec3 delta = vel * dt;

    Vec3 nx = {pos.x + delta.x, pos.y, pos.z};
    if (!collidesAt(world, nx))
        pos.x = nx.x;
    else
        vel.x = 0;

    Vec3 ny = {pos.x, pos.y + delta.y, pos.z};
    if (!collidesAt(world, ny))
    {
        pos.y = ny.y;
        onGround = false;
    }
    else
    {
        if (vel.y < 0)
            onGround = true;
        vel.y = 0;
    }

    Vec3 nz = {pos.x, pos.y, pos.z + delta.z};
    if (!collidesAt(world, nz))
        pos.z = nz.z;
    else
        vel.z = 0;
}