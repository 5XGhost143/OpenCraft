#version 460 core

in  vec3 v_color;
out vec4 frag_color;

uniform float u_time;

void main()
{
    float shift = 0.5 + 0.5 * sin(u_time * 1.5);
    vec3  col   = mix(v_color, v_color.brg, shift * 0.35);
    frag_color  = vec4(col, 1.0);
}
