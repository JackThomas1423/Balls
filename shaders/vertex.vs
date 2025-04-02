#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 textCord;

out vec3 vertex_color;
out vec2 tex_coords;

void main() {
    gl_Position = vec4(position, 0.0, 1.0);
    vertex_color = color;
    tex_coords = textCord;
}