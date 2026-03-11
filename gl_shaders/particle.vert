#version 460 core

layout(location=0) in vec3 a_pos;
layout(location=1) in vec3 a_color;
layout(location=2) in float a_life;
layout(location=3) in float a_size;

out vec3 v_color;
out float v_alpha;

uniform mat4 u_vp;

void main() {
    gl_Position = u_vp * vec4(a_pos, 1.0);
    gl_PointSize = a_size * 400.0 / gl_Position.w;
    v_color = a_color;
    v_alpha = a_life;
}
