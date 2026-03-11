#version 460 core

in  vec3  v_color;
in  float v_alpha;
out vec4  frag_color;

void main() {
    vec2 uv = gl_PointCoord * 2.0 - 1.0;
    float d = dot(uv, uv);
    if (d > 1.0) discard;
    frag_color = vec4(v_color, v_alpha * (1.0 - d * 0.5));
}
