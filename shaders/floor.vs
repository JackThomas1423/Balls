#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 foo;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = view * model * projection * vec4(pos, 1.0);
}