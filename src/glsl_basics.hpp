#ifndef GLSL_BASICS_HPP
#define GLSL_BASICS_HPP

#include <vector>
#include <regex>

namespace glsl_basics {

static const char* vertex_regex = "^in (vec[0-9]|float) [a-zA-Z0-9]+(?:\\[([0-9]+)\\])?;";
static const char* struct_regex = "struct\\s+(\\w+)\\s*\\{(\\s*(?:\\w+\\s+\\w+\\s*(?:\\[\\d+\\])?;\\s+)*)\\}\\s*(\\w+)?\\s*;";
static const char* struct_part_regex = "(\\w+)\\s+(\\w+)\\s*(?:\\[(\\d+)\\])?;";

enum class ShaderType {
    Float = 1,
    Vec2,
    Vec3,
    Vec4,
};

struct ShaderStruct {
    std::string name;
    std::vector<std::pair<ShaderType, std::string>> components;
    int size;
};

}

#endif