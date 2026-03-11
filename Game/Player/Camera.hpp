#pragma once
#include "../../Engine/Core/Math.hpp"
#include <cmath>
#include <algorithm>

class Camera {
public:
    Vec3  pos   = {16.f, 74.f, 16.f};
    float yaw   = 0.f;
    float pitch = 0.f;

    int XPlayer_FOV = 30;

    static constexpr float PI = 3.14159265f;

    Vec3 forward() const {
        return {
            std::cos(pitch) * std::sin(yaw),
            std::sin(pitch),
            std::cos(pitch) * -std::cos(yaw)
        };
    }

    Vec3 right() const {
        return {std::cos(yaw), 0.f, std::sin(yaw)};
    }

    void rotate(float dx, float dy, float sens = 0.002f) {
        yaw   += dx * sens;
        pitch -= dy * sens;
        if (pitch >  PI*0.499f) pitch =  PI*0.499f;
        if (pitch < -PI*0.499f) pitch = -PI*0.499f;
    }

    void setFOV(int fov) {
        XPlayer_FOV = std::clamp(fov, 30, 110);
    }

    Mat4 viewMatrix() const {
        Vec3 f = forward();
        Vec3 up = {0,1,0};
        return Mat4::lookAt(pos, pos + f, up);
    }

    Mat4 projectionMatrix(float aspect, float nearPlane = 0.1f, float farPlane = 1000.f) const {
        float fovRad = XPlayer_FOV * (PI / 180.f);
        return Mat4::perspective(fovRad, aspect, nearPlane, farPlane);
    }
};