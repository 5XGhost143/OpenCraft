#version 460 core

in  vec3 v_color;
in  float v_fog;
out vec4 frag_color;

uniform float u_time;

void main() {
    vec3 skyColor = vec3(0.45, 0.68, 0.95);
    vec3 col = mix(v_color, skyColor, v_fog);
    frag_color = vec4(col, 1.0);
}
