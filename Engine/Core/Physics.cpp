#include "Physics.hpp"
#include <cmath>

Vec3 Player::eyePos() const
{
    return {pos.x, pos.y + EYE_HEIGHT, pos.z};
}

bool Player::collidesAt(const IWorldReader& world, Vec3 p) const
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

void Player::applyFriction(float dt)
{
    float speed = std::sqrt(vel.x * vel.x + vel.z * vel.z);
    if (speed < 0.01f)
    {
        vel.x = 0.f;
        vel.z = 0.f;
        return;
    }

    float drop      = 0.f;
    float control   = speed < STOP_SPEED ? STOP_SPEED : speed;
    drop           += control * GROUND_FRICTION * dt;

    float newSpeed  = speed - drop;
    if (newSpeed < 0.f) newSpeed = 0.f;
    newSpeed /= speed;

    vel.x *= newSpeed;
    vel.z *= newSpeed;
}

void Player::accelerate(Vec3 wishDir, float wishSpeed, float accel, float dt)
{
    float currentSpeed = vel.x * wishDir.x + vel.z * wishDir.z;
    float addSpeed     = wishSpeed - currentSpeed;

    if (addSpeed <= 0.f)
        return;

    float accelSpeed = accel * wishSpeed * dt;
    if (accelSpeed > addSpeed)
        accelSpeed = addSpeed;

    vel.x += accelSpeed * wishDir.x;
    vel.z += accelSpeed * wishDir.z;
}

void Player::update(float dt, const IWorldReader& world, Vec3 wishDir, bool jumpPressed)
{
    float wishSpeed = MAX_SPEED;

    if (wishDir.x != 0.f || wishDir.z != 0.f)
    {
        float len = std::sqrt(wishDir.x * wishDir.x + wishDir.z * wishDir.z);
        wishDir.x /= len;
        wishDir.z /= len;
    }
    else
    {
        wishSpeed = 0.f;
    }

    if (onGround)
    {
        applyFriction(dt);
        accelerate(wishDir, wishSpeed, GROUND_ACCEL, dt);

        if (jumpPressed)
        {
            vel.y    = JUMP_SPEED;
            onGround = false;
        }
    }
    else
    {
        float airWish = wishSpeed;
        if (airWish > AIR_CAP) airWish = AIR_CAP;
        accelerate(wishDir, airWish, AIR_ACCEL, dt);

        vel.y -= GRAVITY * dt;
    }

    Vec3 delta = vel * dt;

    Vec3 nx = {pos.x + delta.x, pos.y, pos.z};
    if (!collidesAt(world, nx))
        pos.x = nx.x;
    else
        vel.x = 0.f;

    Vec3 ny = {pos.x, pos.y + delta.y, pos.z};
    if (!collidesAt(world, ny))
    {
        pos.y    = ny.y;
        onGround = false;
    }
    else
    {
        if (vel.y < 0.f)
            onGround = true;
        vel.y = 0.f;
    }

    Vec3 nz = {pos.x, pos.y, pos.z + delta.z};
    if (!collidesAt(world, nz))
        pos.z = nz.z;
    else
        vel.z = 0.f;
}