#pragma once
#include "Types.hpp"
#include "IWorldReader.hpp"

constexpr float PLAYER_WIDTH    = 0.6f;
constexpr float PLAYER_HEIGHT   = 1.8f;
constexpr float EYE_HEIGHT      = 1.62f;
constexpr float HALF_W          = PLAYER_WIDTH * 0.5f;

constexpr float GRAVITY         = 800.f;
constexpr float JUMP_SPEED      = 180.f;
constexpr float MAX_SPEED       = 200.f;
constexpr float GROUND_ACCEL    = 7.f;
constexpr float GROUND_FRICTION = 6.f;
constexpr float AIR_ACCEL       = 1.f;
constexpr float AIR_CAP         = 30.f;
constexpr float STOP_SPEED      = 100.f;

struct Player
{
    Vec3 pos      = {16.f, 74.f, 16.f};
    Vec3 vel      = {0.f, 0.f, 0.f};
    bool onGround = false;

    Vec3 eyePos() const;
    bool collidesAt(const IWorldReader& world, Vec3 p) const;
    void update(float dt, const IWorldReader& world, Vec3 wishDir, bool jumpPressed);

private:
    void applyFriction(float dt);
    void accelerate(Vec3 wishDir, float wishSpeed, float accel, float dt);
};