#version 460 core

layout(location=0) in vec3 a_pos;
layout(location=1) in vec3 a_color;
layout(location=2) in float a_ao;

out vec3 v_color;
out float v_fog;

uniform mat4 u_vp;
uniform vec3 u_camPos;
uniform float u_breakAnim;
uniform vec3  u_breakPos;

void main() {
    vec3 pos = a_pos;

    float distToBreak = length(pos - u_breakPos - vec3(0.5));
    if (distToBreak < 0.87 && u_breakAnim > 0.0) {
        float p   = u_breakAnim;
        float amp = 0.012 + p * p * 0.03;

        pos.x += sin(p * 22.0)        * amp;
        pos.y += sin(p * 17.0 + 1.0)  * amp * 0.5;
        pos.z += sin(p * 19.0 + 2.5)  * amp;

        float crack = p * p * 0.025;
        pos += normalize(pos - (u_breakPos + vec3(0.5))) * crack;
    }

    gl_Position = u_vp * vec4(pos, 1.0);
    v_color = a_color * a_ao;

    float dist = length(pos - u_camPos);
    v_fog = clamp((dist - 20.0) / 14.0, 0.0, 1.0);
}
