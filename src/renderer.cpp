#include <vector>
#include <numeric>
#include "shader.hpp"
#include "renderer.hpp"

Renderer::Renderer() : RSO("shaders/vertex.vs","shaders/fragment.fs") {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);

    glBindVertexArray(VAO);
}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Renderer::BindBufferData(std::vector<float> vertices, std::vector<unsigned int> indices) {
    int bitwidth = RSO.getLocationOffset(RSO.getLocationLength() - 1);
    TAC = vertices.size() / bitwidth;

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    int width = 0;
    int index = 0;
    while(width < bitwidth) {
        int segmentwidth = RSO.getLocationOffset(index) - RSO.getLocationOffset(index - 1);
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, segmentwidth, GL_FLOAT, GL_FALSE, bitwidth * sizeof(float), (void*)(width * sizeof(float)));
        width += segmentwidth;
        ++index;
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::Render() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, TAC);
}