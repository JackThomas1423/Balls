#version 330 core
out vec4 FragColor;

in vec3 color;

void main() {
    // Set the color of the fragment
    FragColor = vec4(color, 1.0);
}