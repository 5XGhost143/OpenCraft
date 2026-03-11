#pragma once
#include <cstdint>
#include <cmath>

struct Vec2 { float x, y; };
struct Vec3 {
    float x, y, z;
    Vec3 operator+(const Vec3& o) const { return {x+o.x, y+o.y, z+o.z}; }
    Vec3 operator-(const Vec3& o) const { return {x-o.x, y-o.y, z-o.z}; }
    Vec3 operator*(float s)       const { return {x*s, y*s, z*s}; }
    Vec3& operator+=(const Vec3& o) { x+=o.x; y+=o.y; z+=o.z; return *this; }
    Vec3& operator-=(const Vec3& o) { x-=o.x; y-=o.y; z-=o.z; return *this; }
    float dot(const Vec3& o) const { return x*o.x + y*o.y + z*o.z; }
    Vec3 cross(const Vec3& o) const {
        return {y*o.z - z*o.y, z*o.x - x*o.z, x*o.y - y*o.x};
    }
    float length() const { return std::sqrt(x*x + y*y + z*z); }
    Vec3 normalized() const { float l = length(); return l > 0 ? Vec3{x/l,y/l,z/l} : Vec3{0,0,0}; }
};
struct Vec4 { float x, y, z, w; };
struct IVec3 {
    int x, y, z;
    bool operator==(const IVec3& o) const { return x==o.x && y==o.y && z==o.z; }
};

using BlockID = uint8_t;

constexpr BlockID BLOCK_AIR    = 0;
constexpr BlockID BLOCK_GRASS  = 1;
constexpr BlockID BLOCK_DIRT   = 2;
constexpr BlockID BLOCK_STONE  = 3;
constexpr BlockID BLOCK_BEDROCK = 4;