#version 330 core
out vec4 FragColor;

in vec2 TexCord;

uniform sampler2D ourTexture;

void main() {
    FragColor = texture(ourTexture, TexCord) * vec4(0.8, 0.1, 0.6, 1.0);
}