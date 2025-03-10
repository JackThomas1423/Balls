#include <glad/glad.h>
#include <iostream>
#include <vector>

namespace shader {

    struct Position {
        float x;
        float y;
    };

    struct Color {
        float r;
        float g;
        float b;
    };

    struct LightMap {
        float intensity;
    };

    struct Vertex {
        Position pos;
        Color color;
        LightMap lightMap;
    };

    unsigned int compileShaderCode(const char* code, GLenum type);
    unsigned int createShaderProgram(const char* vertexCode, const char* fragmentCode);
};