#include <glad/glad.h>
#include <iostream>
#include <vector>

namespace shader {

    unsigned int compileShaderCode(const char* code, GLenum type);
    unsigned int createShaderProgram(const char* vertexCode, const char* fragmentCode);
};