#version 330 core
out vec4 FragColor;

in vec3 vertex_color;
in vec2 TexCord;

uniform sampler2D ourTexture;

void main() {
    FragColor = texture(ourTexture, TexCord) * vec4(vertex_color, 1.0);
}