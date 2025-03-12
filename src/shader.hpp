#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <regex>

namespace shader {

    static const char* LayoutRegexPattern = "layout\\s*\\(\\s*location\\s*\\=\\s*\\d+\\s*\\)\\s*in\\s+(\\w+)\\s+(\\w+)\\s*(?:\\[(\\d+)\\])?\\s*;";

    enum class GLSL_TYPE {
        FLOAT = 0,
        INT = 1,
        VEC2 = 2,
        VEC3 = 3,
        VEC4 = 4,
        MAT2 = 5,
        MAT3 = 6,
        MAT4 = 7,
    };

    GLSL_TYPE glsl_type(std::string s);

    struct VertexLayout {
        std::vector<GLSL_TYPE> types;
        std::vector<unsigned int> sizes;
        VertexLayout(std::vector<GLSL_TYPE> types, std::vector<unsigned int> sizes) : types(types), sizes(sizes) {}

        inline GLSL_TYPE type(int index) { return types[index]; }
        unsigned int size(int index) {
            switch (types[index]) {
                case GLSL_TYPE::FLOAT:return 1;
                case GLSL_TYPE::INT:return 1;
                case GLSL_TYPE::VEC2:return 2;
                case GLSL_TYPE::VEC3:return 3;
                case GLSL_TYPE::VEC4:return 4;
                case GLSL_TYPE::MAT2:return 4;
                case GLSL_TYPE::MAT3:return 9;
                case GLSL_TYPE::MAT4:return 16;
                default:
                    return 0;
            }
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