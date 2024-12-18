#include "shader.hpp"

class Renderer {
    public:
    Shader RSO;
    unsigned int VBO, VAO, IBO;
    size_t TAC;
    Renderer();
    void BindBufferData(std::vector<float> vertices, std::vector<unsigned int> indices);
    void Render();
};