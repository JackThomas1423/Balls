#ifndef GLSL_BASICS_HPP
#define GLSL_BASICS_HPP

#include <vector>
#include <numeric>
#include <regex>

namespace glsl_basics {

static const char* layout_regex = "in\\s+(\\w+)\\s+(\\w+)\\s*(?:\\[(\\d+)\\])?\\s*;";
static const char* struct_regex = "struct\\s+(\\w+)\\s*\\{(\\s*(?:\\w+\\s+\\w+\\s*(?:\\[\\d+\\])?;\\s+)*)\\}\\s*(\\w+)?\\s*;";
static const char* struct_part_regex = "(\\w+)\\s+(\\w+)\\s*(?:\\[(\\d+)\\])?;";

enum class ShaderType {
    Unknown = 0,
    Struct,
    Float,
    Vec2,
    Vec3,
    Vec4,
};

struct ShaderInput {
    int size;
    ShaderType type;
    std::string name;
    std::optional<int> arraySize;
    ShaderInput(ShaderType _type_, std::string _name_, std::optional<int> _arraySize_, int _size_) : size(_size_), type(_type_), name(_name_), arraySize(_arraySize_) {}
};

struct ShaderStruct {
    std::string name;
    std::vector<ShaderInput> inputs;
    ShaderStruct(std::string _name_, std::vector<ShaderInput> _inputs_) : name(_name_), inputs(_inputs_) {}
};

}

#endif