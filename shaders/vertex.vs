#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;

struct test {
    vec3 a[3];
    vec2 b;
};

uniform vec2 translation;

out vec3 vertex_color;

void main() {
    gl_Position = vec4(position, 0.0, 1.0);
    vertex_color = color;
}