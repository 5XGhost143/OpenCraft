#pragma once
#include <cstdint>
#include <cmath>

struct Vec2
{
    float x, y;
};

struct Vec3
{
    float x, y, z;

    Vec3 operator+(const Vec3& o) const;
    Vec3 operator-(const Vec3& o) const;
    Vec3 operator*(float s) const;
    Vec3& operator+=(const Vec3& o);
    Vec3& operator-=(const Vec3& o);
    float dot(const Vec3& o) const;
    Vec3 cross(const Vec3& o) const;
    float length() const;
    Vec3 normalized() const;
};

struct Vec4
{
    float x, y, z, w;
};

struct IVec3
{
    int x, y, z;

    bool operator==(const IVec3& o) const;
};

using BlockID = uint8_t;

constexpr BlockID BLOCK_AIR     = 0;
constexpr BlockID BLOCK_GRASS   = 1;
constexpr BlockID BLOCK_DIRT    = 2;
constexpr BlockID BLOCK_STONE   = 3;
constexpr BlockID BLOCK_BEDROCK = 4;