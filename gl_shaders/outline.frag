#version 460 core

out vec4 frag_color;
uniform float u_breakAnim;

void main() {
    float r = mix(0.0, 1.0, u_breakAnim);
    frag_color = vec4(r, 1.0 - u_breakAnim * 0.5, 0.0, 0.85);
}
