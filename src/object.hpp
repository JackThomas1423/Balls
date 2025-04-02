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
            unsigned int shaderProgram;
            shader::VertexLayout vertexLayout;
        public:
            DataObject(shader::VertexLayout layout, unsigned int program, const std::vector<float>& vertices, const std::vector<unsigned int>& indices) : VAO(0), VBO(0), EBO(0), indexCount(indices.size()), shaderProgram(program), vertexLayout(layout) {
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

            void setShaderProgram(shader::VertexLayout layout, unsigned int program) {
                vertexLayout = layout;
                shaderProgram = program;
            }

            void updateAttributes() {
                glBindVertexArray(VAO);

                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

                unsigned int stride = vertexLayout.stride();
                for (size_t i = 0; i < vertexLayout.types.size(); ++i) {
                    unsigned int loc = vertexLayout.location(i);
                    unsigned int vertexSize = vertexLayout.vertex(i);
                    glVertexAttribPointer(loc, vertexSize, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(vertexLayout.stride(i) * sizeof(float)));
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
                glUseProgram(shaderProgram);
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

    class Texture : public DataObject {
        private:
            unsigned int texture;
        public:
            Texture(shader::VertexLayout layout, unsigned int program, const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const char* texturePath) : DataObject(layout, program, vertices, indices), texture(0) {
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                int width, height, nrChannels;
                unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
                if (data) {
                    GLenum format;
                    if (nrChannels == 1)
                        format = GL_RED;
                    else if (nrChannels == 3)
                        format = GL_RGB;
                    else if (nrChannels == 4)
                        format = GL_RGBA;

                    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                    glGenerateMipmap(GL_TEXTURE_2D);
                } else {
                    std::cout << "Failed to load texture" << std::endl;
                }
                stbi_image_free(data);
            }

            void draw() {
                glBindTexture(GL_TEXTURE_2D, texture);
                DataObject::draw();
            }

            void cleanup() {
                if (texture) glDeleteTextures(1, &texture);
                DataObject::cleanup();
            }

            ~Texture() {
                cleanup();
            }
    };
};