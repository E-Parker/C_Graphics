#version 460 core

in vec3 v_position;
in vec3 v_normal;
in vec2 v_tcoord;

out vec4 FragColor;

void main() {
    vec3 normal = (v_normal + vec3(0.5, 0.5, 0.5)) * 0.5;
    FragColor = vec4(normal, 1.0);
}
