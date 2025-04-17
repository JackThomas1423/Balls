#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shader.hpp"
#include "texture.hpp"

namespace Object {

    struct Mesh {
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        std::vector<glm::vec3> vertices;
        std::vector<unsigned int> indices;

        std::vector<float> vertexData() {
            std::vector<float> data;
            for (const auto& vertex : vertices) {
                data.push_back(vertex.x);
                data.push_back(vertex.y);
                data.push_back(vertex.z);
                data.push_back(1.0f);
                data.push_back(0.0f);
                data.push_back(0.0f);
            }
            return data;
        }

        std::vector<unsigned int> indexData() { return indices; }

    };

    namespace Shape {
        Mesh createSphere(float radius, int numSegments) {
            std::vector<glm::vec3> vertices;
            std::vector<unsigned int> indices;

            float pi = acos(-1.0f);
            for (int i = 0; i <= numSegments; ++i) {
                float phi = pi * i / numSegments;
                for (int j = 0; j <= numSegments; ++j) {
                    float theta = 2 * pi * j / numSegments;
                    glm::vec3 vertex(
                        radius * sin(phi) * cos(theta),
                        radius * sin(phi) * sin(theta),
                        radius * cos(phi)
                    );
                    vertices.push_back(vertex);
                }
            }
            for (int i = 0; i < numSegments; ++i) {
                for (int j = 0; j < numSegments; ++j) {
                    unsigned int first = i * (numSegments + 1) + j;
                    unsigned int second = first + numSegments + 1;

                    indices.push_back(first);
                    indices.push_back(second);
                    indices.push_back(first + 1);

                    indices.push_back(second);
                    indices.push_back(second + 1);
                    indices.push_back(first + 1);
                }
            }
            return Mesh { glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), vertices, indices };
        }
    }

    class Camera {
        private:
            glm::vec3 position;
            glm::vec3 front;
            glm::vec3 up;
            float fov;
        public:
            Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up, float fov) : position(pos), front(front), up(up), fov(fov) {}

            void translate(glm::vec3 offset) { position += offset; }

            void setPosition(glm::vec3 pos) { position = pos; }
            void setFront(glm::vec3 f) { front = f; }
            void setUp(glm::vec3 u) { up = u; }
            void setFov(float f) { fov = f; }

            float getFov() { return fov; }

            glm::vec3 getPosition() { return position; }
            glm::vec3 getFront() { return front; }
            glm::vec3 getUp() { return up; }
            glm::mat4 getViewMatrix() { return glm::lookAt(position, position + front, up); }
            glm::mat4 getProjectionMatrix(float aspectRatio) { return glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f); }
            glm::mat4 getProjectionMatrix(float aspectRatio, float nearPlane, float farPlane) { return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane); }
    };

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

            unsigned int getShaderProgram() { return shaderProgram; }

            unsigned int getVAO() { return VAO; }
            unsigned int getVBO() { return VBO; }
            unsigned int getEBO() { return EBO; }

            unsigned int getIndexCount() { return indexCount; }

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
            Texture(shader::VertexLayout layout, unsigned int program, const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const char* texturePath)
                : DataObject(layout, program, vertices, indices), texture(texture::loadTexture(texturePath, GL_REPEAT, GL_LINEAR)) {}

            void draw() {
                glUseProgram(DataObject::getShaderProgram());
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture);
                glBindVertexArray(DataObject::getVAO());
                glDrawElements(GL_TRIANGLES, DataObject::getIndexCount(), GL_UNSIGNED_INT, 0);
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