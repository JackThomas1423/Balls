#version 330 core
in vec2 position;
in vec3 color;

uniform vec2 translation;

out vec3 vertex_color;

void main() {
    mat2 rot = mat2(
        1, tan(translation.x),
        tan(translation.x), 1
    );

    gl_Position = vec4(position * rot, 0.0, 1.0);
    vertex_color = color;
}