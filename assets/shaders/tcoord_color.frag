#version 460 core

in vec2 v_tcoord;

out vec4 FragColor;

void main()
{
    FragColor = vec4(v_tcoord, 0.0, 1.0);
}
