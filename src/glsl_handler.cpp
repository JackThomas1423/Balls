#include "glsl_handler.hpp"
#include <iostream>

namespace glsl_handler {

std::vector<std::smatch> applyRexex(const std::string& code, const std::string& regex) {
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

std::string insertRegex(const std::string& code, const std::string& regex, const std::string& insertText) {
    std::regex pattern(regex);
    std::sregex_iterator chunk_start = std::sregex_iterator(code.begin(), code.end(), pattern);
    std::sregex_iterator chunk_end = std::sregex_iterator();

    std::string modifiedCode = code;
    std::ptrdiff_t offset = 0;

    for (std::sregex_iterator i = chunk_start; i != chunk_end; ++i) {
        std::smatch match = *i;
        modifiedCode.insert(match.position() + offset, insertText);
        offset += insertText.length();
    }
    return modifiedCode;
}

glsl_basics::ShaderType stringToShaderType(const std::string& type, const std::vector<glsl_basics::ShaderStruct>& structs) {
    if (type == "float") {
        return glsl_basics::ShaderType(1, glsl_basics::ShaderType::Float, 1);
    } else if (type == "vec2") {
        return glsl_basics::ShaderType(2, glsl_basics::ShaderType::Vec2, 1);
    } else if (type == "vec3") {
        return glsl_basics::ShaderType(3, glsl_basics::ShaderType::Vec3, 1);
    } else if (type == "vec4") {
        return glsl_basics::ShaderType(4, glsl_basics::ShaderType::Vec4, 1);
    } else {
        for (auto structType : structs) {
            if (structType.name == type) {
                return glsl_basics::ShaderType(structType.size(), glsl_basics::ShaderType::Struct, structType.offset());
            }
        }
    }
    return glsl_basics::ShaderType(0, glsl_basics::ShaderType::Unknown, 0);
}

}