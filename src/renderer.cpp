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
    int bitwidth = RSO.bitWidth();
    TAC = vertices.size() / bitwidth;

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    int width = 0;
    int index = 0;
    while(width < bitwidth) {
        int segmentwidth = RSO.bitSize(index);
        GLenum type;
        size_t size_mult = 0;
        switch (RSO.getShaderType(index)) {
            case ShaderType::Float:case ShaderType::Vec2:case ShaderType::Vec3:case ShaderType::Vec4:
                type = GL_FLOAT;
                size_mult = sizeof(float);
                break;
            case ShaderType::Int:
                type = GL_INT;
                size_mult = sizeof(int);
                break;
            default:
                std::cout << "ERROR::RENDERER::VERTEX::INTERPRETATION_FAILED" << std::endl;
                std::cout << "ERROR: unknown ShaderType cannot load" << std::endl;
                exit(1);
                break;
        }
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, segmentwidth, type, GL_FALSE, bitwidth * size_mult, (void*)(width * size_mult));
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