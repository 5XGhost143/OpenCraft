#pragma once
#include "../../Engine/Core/Types.hpp"
#include <vector>

struct Particle
{
    Vec3  pos;
    Vec3  vel;
    float r;
    float g;
    float b;
    float life;
    float maxLife;
    float size;
};

class ParticleSystem
{
public:
    std::vector<Particle> particles;

    void spawnBreak(Vec3 center, float r, float g, float b);
    void spawnPlace(Vec3 center, float r, float g, float b);
    void update(float dt);
};