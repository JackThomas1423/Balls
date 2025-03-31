#include "shader.hpp"

namespace object {
    struct DisplayObject {
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        unsigned int vertexCount;
        DisplayObject() {
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);
        }

        ~DisplayObject() {
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
        }

        void bind(shader::VertexLayout vertexLayout, std::vector<float> vertices, std::vector<unsigned int> indices, GLenum usage = GL_STATIC_DRAW) {
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), usage);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), usage);

            int stride = vertexLayout.stride() * sizeof(float);

            int index = 0;
            while (vertexLayout.types.size() > index) {
                glVertexAttribPointer(vertexLayout.location(index), vertexLayout.vertex(index), GL_FLOAT, GL_FALSE, stride, (void*)(vertexLayout.stride(index) * sizeof(float)));
                glEnableVertexAttribArray(index);
                index++;
            }

            vertexCount = indices.size();
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        void draw() {
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
        }
    };
};