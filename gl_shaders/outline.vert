#version 460 core

layout(location=0) in vec3 a_pos;

uniform mat4 u_vp;
uniform vec3 u_blockPos;
uniform float u_time;

void main() {
    float pulse = 1.0 + 0.015 * sin(u_time * 6.0);
    vec3 center = u_blockPos + vec3(0.5);
    vec3 pos = center + (a_pos - vec3(0.5)) * pulse;
    gl_Position = u_vp * vec4(pos, 1.0);
}
