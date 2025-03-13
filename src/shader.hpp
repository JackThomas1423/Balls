#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <regex>

namespace shader {

    static const char* LayoutRegexPattern = "layout\\s*\\(\\s*location\\s*\\=\\s*\\d+\\s*\\)\\s*in\\s+(\\w+)\\s+(\\w+)\\s*(?:\\[(\\d+)\\])?\\s*;";

    struct VertexLayout {
        std::vector<std::string> types;
        std::vector<unsigned int> sizes;
        VertexLayout(std::vector<std::string> types, std::vector<unsigned int> sizes) : types(types), sizes(sizes) {}

        unsigned int vertex(int index) {
            std::string s = types[index];
            if (s == "int") return 1;
            if (s == "float") return 1;
            if (s == "vec2") return 2;
            if (s == "vec3") return 3;
            if (s == "vec4") return 4;
            if (s == "mat2") return 4;
            if (s == "mat3") return 9;
            if (s == "mat4") return 16;
            return -1;
        }

        unsigned int stride() {
            unsigned int stride = 0;
            for (int i = 0; i < sizes.size(); i++) {
                stride += sizes[i] * vertex(i);
            }
            return stride;
        }

        unsigned int location(int index) {
            unsigned int loc = 0;
            for (int i = 0; i < index; i++) {
                loc += sizes[i];
            }
            return loc;
        }
    };

    VertexLayout parseVertexShaderCode(const char* code);

    unsigned int compileShaderCode(const char* code, GLenum type);
    unsigned int createShaderProgram(const char* vertexCode, const char* fragmentCode);

    std::string getShaderCode(const char* path);
};