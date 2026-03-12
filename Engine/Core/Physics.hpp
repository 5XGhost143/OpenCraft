#pragma once
#include "Types.hpp"

class World;

constexpr float PLAYER_WIDTH  = 0.6f;
constexpr float PLAYER_HEIGHT = 1.8f;
constexpr float EYE_HEIGHT    = 1.62f;
constexpr float GRAVITY       = 28.f;
constexpr float JUMP_SPEED    = 9.5f;
constexpr float MOVE_SPEED    = 5.0f;
constexpr float HALF_W        = PLAYER_WIDTH * 0.5f;

struct Player
{
    Vec3 pos      = {16.f, 74.f, 16.f};
    Vec3 vel      = {0, 0, 0};
    bool onGround = false;

    Vec3 eyePos() const;
    bool collidesAt(const World& world, Vec3 p) const;
    void update(float dt, const World& world, Vec3 wishMove, bool jumpPressed);
};