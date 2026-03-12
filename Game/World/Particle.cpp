#include "Particle.hpp"
#include <algorithm>
#include <cstdlib>
#include <cmath>

void ParticleSystem::spawnBreak(Vec3 center, float r, float g, float b)
{
    for (int i = 0; i < 24; ++i)
    {
        float rx    = (rand() % 1000 / 500.f - 1.f);
        float ry    = (rand() % 1000 / 500.f);
        float rz    = (rand() % 1000 / 500.f - 1.f);
        float speed = 2.f + rand() % 1000 / 500.f;
        Vec3  vel   = Vec3{rx, ry, rz}.normalized() * speed;
        float life  = 0.3f + rand() % 1000 / 2000.f;
        float sz    = 0.06f + rand() % 100 / 1000.f;
        particles.push_back({center, vel, r, g, b, life, life, sz});
    }
}

void ParticleSystem::spawnPlace(Vec3 center, float r, float g, float b)
{
    for (int i = 0; i < 12; ++i)
    {
        float rx    = (rand() % 1000 / 500.f - 1.f) * 0.5f;
        float ry    = 1.f + rand() % 1000 / 1000.f;
        float rz    = (rand() % 1000 / 500.f - 1.f) * 0.5f;
        float speed = 1.5f + rand() % 1000 / 1000.f;
        Vec3  vel   = Vec3{rx, ry, rz}.normalized() * speed;
        float life  = 0.25f + rand() % 1000 / 2000.f;
        particles.push_back({center, vel, r, g, b, life, life, 0.05f});
    }
}

void ParticleSystem::update(float dt)
{
    for (auto& p : particles)
    {
        p.vel.y -= 8.f * dt;
        p.pos   += p.vel * dt;
        p.life  -= dt;
    }

    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
            [](const Particle& p) { return p.life <= 0; }),
        particles.end()
    );
}