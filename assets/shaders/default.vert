#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTcoord;

struct Light {
  vec3 position;        // 16   0
  vec3 direction;       // 16   16
  vec3 color;           // 16   32
  float attenuation;    // 4    48
};

layout (std140, binding = 0) uniform FrameData {
    mat4 u_view;
    vec3 u_position;
    vec3 u_direction;
    float u_time;
};

uniform mat4 u_mvp;

out vec3 v_position;
out vec3 v_normal;    
out vec2 v_tcoord;
out float v_time;

void main() {
  v_position = (u_mvp * vec4(aPosition, 1.0)).xyz;
  v_normal = aNormal;
  v_tcoord = aTcoord;
  v_time = u_time;
  gl_Position = u_view * u_mvp * vec4(aPosition, 1.0);
}
