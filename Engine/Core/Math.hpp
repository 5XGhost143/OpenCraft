#pragma once
#include "Types.hpp"

struct Mat4
{
    float m[16] = {};

    static Mat4 identity();
    static Mat4 perspective(float fovY, float aspect, float near, float far);
    static Mat4 lookAt(Vec3 eye, Vec3 center, Vec3 up);
    static Mat4 translation(Vec3 t);
    static Mat4 scale(Vec3 s);

    Mat4 operator*(const Mat4& b) const;
};