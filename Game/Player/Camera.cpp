#include "Camera.hpp"
#include <cmath>
#include <algorithm>

Vec3 Camera::forward() const
{
    return {
        std::cos(pitch) * std::sin(yaw),
        std::sin(pitch),
        std::cos(pitch) * -std::cos(yaw)
    };
}

Vec3 Camera::right() const
{
    return {std::cos(yaw), 0.f, std::sin(yaw)};
}

void Camera::rotate(float dx, float dy, float sens)
{
    yaw   += dx * sens;
    pitch -= dy * sens;

    if (pitch >  PI * 0.499f) pitch =  PI * 0.499f;
    if (pitch < -PI * 0.499f) pitch = -PI * 0.499f;
}

void Camera::setFOV(int fov)
{
    XPlayer_FOV = std::clamp(fov, 30, 110);
}

Mat4 Camera::viewMatrix() const
{
    Vec3 f  = forward();
    Vec3 up = {0, 1, 0};
    return Mat4::lookAt(pos, pos + f, up);
}

Mat4 Camera::projectionMatrix(float aspect, float nearPlane, float farPlane) const
{
    float fovRad = XPlayer_FOV * (PI / 180.f);
    return Mat4::perspective(fovRad, aspect, nearPlane, farPlane);
}