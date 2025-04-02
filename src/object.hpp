#include <glad/glad.h>
#include "shader.hpp"

namespace Object {

    // DataObject class used to encapsulate the OpenGL VAO, VBO, and EBO for rendering
    class DataObject {
        private:
            unsigned int VAO;
            unsigned int VBO;
            unsigned int EBO;
            unsigned int indexCount;
        public:
            DataObject(shader::VertexLayout layout, const std::vector<float>& vertices, const std::vector<unsigned int>& indices) : VAO(0), VBO(0), EBO(0), indexCount(indices.size()) {
                glGenVertexArrays(1, &VAO);
                glGenBuffers(1, &VBO);
                glGenBuffers(1, &EBO);

                glBindVertexArray(VAO);

                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

                unsigned int stride = layout.stride();
                for (size_t i = 0; i < layout.types.size(); ++i) {
                    unsigned int loc = layout.location(i);
                    unsigned int vertexSize = layout.vertex(i);
                    glVertexAttribPointer(loc, vertexSize, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(layout.stride(i) * sizeof(float)));
                    glEnableVertexAttribArray(loc);
                }

                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindVertexArray(0);
            }

            void bind(std::vector<float> vertices) {
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
            }

            void bind(std::vector<unsigned int> indices) {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

                indexCount = indices.size();
            }

            // needs you to use glUseProgram(shaderProgram) before calling draw()
            void draw() {
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
            }

            void cleanup() {
                if (EBO) glDeleteBuffers(1, &EBO);
                if (VBO) glDeleteBuffers(1, &VBO);
                if (VAO) glDeleteVertexArrays(1, &VAO);
            }

            ~DataObject() {
                cleanup();
            }
    };

    //main object class used for rendering raw vertices and indices using shaders
    class Object : public DataObject {
        private:
            shader::VertexLayout vertexLayout;
            unsigned int shaderProgram;
        public:
            Object(shader::VertexLayout layout, const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
                : DataObject(layout, vertices, indices), vertexLayout(layout), shaderProgram(0) {}

            void setShaderProgram(unsigned int program) { shaderProgram = program; }

            // draw function that uses the shader program set by setShaderProgram
            void draw() {
                if (shaderProgram) {
                    glUseProgram(shaderProgram);
                }
                DataObject::draw();
            }

            ~Object() {
                cleanup();
            }
    };
};