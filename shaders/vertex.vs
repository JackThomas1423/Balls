#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 textCord;

out vec3 vertex_color;
out vec2 TexCord;

uniform mat4 transform;

void main() {
    gl_Position = transform * vec4(position, 0.0, 1.0);
    vertex_color = color;
    TexCord = vec2(textCord.x,textCord.y);
}