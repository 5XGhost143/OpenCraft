#pragma once
#include "../../Engine/Core/Math.hpp"

class Camera
{
public:
    Vec3  pos         = {16.f, 74.f, 16.f};
    float yaw         = 0.f;
    float pitch       = 0.f;
    int   XPlayer_FOV = 30;

    static constexpr float PI = 3.14159265f;

    Vec3 forward() const;
    Vec3 right() const;
    void rotate(float dx, float dy, float sens = 0.002f);
    void setFOV(int fov);
    Mat4 viewMatrix() const;
    Mat4 projectionMatrix(float aspect, float nearPlane = 0.1f, float farPlane = 1000.f) const;
};