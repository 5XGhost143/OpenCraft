#version 460 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_color;

out vec3 v_color;

uniform float u_time;
uniform vec2  u_resolution;

void main()
{
    float aspect = u_resolution.x / u_resolution.y;
    float c = cos(u_time), s = sin(u_time);
    vec2 rotated = vec2(c * a_pos.x - s * a_pos.y,
                        s * a_pos.x + c * a_pos.y);
    rotated.x /= aspect;
    gl_Position = vec4(rotated, a_pos.z, 1.0);
    v_color = a_color;
}
