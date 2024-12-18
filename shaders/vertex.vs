#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;

uniform vec2 translation;

out vec3 vertex_color;

void main() {
    gl_Position = vec4(position.x, position.y, 0.0, 1.0) + vec4(translation,0.0,0.0);
    vertex_color = color;
}