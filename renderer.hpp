#include "shader.hpp"

class Renderer {
    public:
    unsigned int VBO, VAO, IBO;
    //Shader RSO;
    Renderer();
    void BindBufferData(std::vector<void> vertet);
};