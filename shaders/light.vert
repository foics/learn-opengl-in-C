#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4x4 model;
uniform mat4x4 view;
uniform mat4x4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
