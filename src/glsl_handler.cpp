#include "glsl_handler.hpp"

template <typename T>
std::vector<T> applyRexex(const std::string& code, const std::string& regex) {
    std::regex pattern(regex);
    std::sregex_iterator chunk_start = std::sregex_iterator(code.begin(), code.end(), pattern);
    std::sregex_iterator chunk_end = std::sregex_iterator();

    std::vector<T> chunks;
    for (std::sregex_iterator i = chunk_start; i != chunk_end; ++i) {
        std::smatch chunk = *i;
        chunks.push_back(chunk);
    }
    return chunks;
}

glsl_basics::ShaderType stringToShaderType(const std::string& type) {
    if (type == "float") {
        return glsl_basics::ShaderType::Float;
    } else if (type == "vec2") {
        return glsl_basics::ShaderType::Vec2;
    } else if (type == "vec3") {
        return glsl_basics::ShaderType::Vec3;
    } else if (type == "vec4") {
        return glsl_basics::ShaderType::Vec4;
    }
    return glsl_basics::ShaderType::Float;
}