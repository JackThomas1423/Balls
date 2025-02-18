#ifndef GLSL_BASICS_HPP
#define GLSL_BASICS_HPP

#include <vector>
#include <regex>

namespace glsl_basics {

static const char* vertex_regex = "^in (vec[0-9]|float) [a-zA-Z0-9]+(?:\\[([0-9]+)\\])?;";
static const char* struct_regex = "struct\\s+(\\w+)\\s*\\{(\\s*(?:\\w+\\s+\\w+\\s*(?:\\[\\d+\\])?;\\s+)*)\\}\\s*(\\w+)?\\s*;";
static const char* struct_part_regex = "(\\w+)\\s+(\\w+)\\s*(?:\\[(\\d+)\\])?;";

enum class ShaderType {
    Unknown = 0,
    Float = 1,
    Vec2,
    Vec3,
    Vec4,
};

struct ShaderStructType {
    ShaderType type;
    std::string name;
    std::optional<int> arraySize;
    ShaderStructType(ShaderType _type_, std::string _name_, std::optional<int> _arraySize_) : type(_type_), name(_name_), arraySize(_arraySize_) {}
};

struct ShaderStruct {
    std::string name;
    std::vector<ShaderStructType> types;
};

}

#endif