#include "glsl_handler.hpp"
#include <iostream>

namespace glsl_handler {

std::vector<std::smatch> applyRegex(const std::string& code, const std::string& regex) {
    std::regex pattern(regex);
    std::sregex_iterator chunk_start = std::sregex_iterator(code.begin(), code.end(), pattern);
    std::sregex_iterator chunk_end = std::sregex_iterator();

    std::vector<std::smatch> chunks;
    for (std::sregex_iterator i = chunk_start; i != chunk_end; ++i) {
        std::smatch chunk = *i;
        chunks.push_back(chunk);
    }
    return chunks;
}

glsl_basics::ShaderType stringToShaderType(const std::string& str) {
    if (str == "float") {
        return glsl_basics::ShaderType::Float;
    } else if (str == "vec2") {
        return glsl_basics::ShaderType::Vec2;
    }
    return glsl_basics::ShaderType::Unknown;
}

int shaderTypeToSize(glsl_basics::ShaderType type) {
    switch(type) {
        case glsl_basics::ShaderType::Float: return 1;
        default: return -1;
    }
}

}