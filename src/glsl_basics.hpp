#ifndef GLSL_BASICS_HPP
#define GLSL_BASICS_HPP

#include <vector>
#include <numeric>
#include <regex>

namespace glsl_basics {

static const char* vertex_regex = "in\\s+(\\w+)\\s+(\\w+)\\s*(?:\\[(\\d+)\\])?\\s*;";
static const char* struct_regex = "struct\\s+(\\w+)\\s*\\{(\\s*(?:\\w+\\s+\\w+\\s*(?:\\[\\d+\\])?;\\s+)*)\\}\\s*(\\w+)?\\s*;";
static const char* struct_part_regex = "(\\w+)\\s+(\\w+)\\s*(?:\\[(\\d+)\\])?;";

struct ShaderType {
    enum Type {
        Unknown,
        Struct,
        Float,
        Vec2,
        Vec3,
        Vec4
    };
    int size;
    Type type;
    int offset;
    ShaderType(int _size_, Type _type_, int _offset_) : size(_size_), type(_type_), offset(_offset_) {}
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
    int offset() {
        return std::accumulate(types.begin(),types.end(),0,[](int acc, ShaderStructType type) {
            if (type.arraySize.has_value()) {
                return acc + type.type.offset * type.arraySize.value();
            }
            return acc + type.type.offset;
        });
    }
    int size() {
        return std::accumulate(types.begin(),types.end(),0,[](int acc, ShaderStructType type) {
            return acc + type.type.size;
        });
    }
};

}

#endif