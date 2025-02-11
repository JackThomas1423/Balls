#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <array>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

static const char* vertex_regex = "^in (vec[0-9]|float) [a-zA-Z0-9]+(?:\\[([0-9]+)\\])?;";
static const char* struct_regex = "struct\\s+(\\w+)\\s*\\{(\\s*(?:\\w+\\s+\\w+\\s*(?:\\[\\d+\\])?;\\s+)*)\\}\\s*(\\w+)?\\s*;";
static const char* struct_part_regex = "(\\w+)\\s+(\\w+)\\s*(?:\\[(\\d+)\\])?;";

enum class ShaderType {
    Float = 1,
    Vec2,
    Vec3,
    Vec4,
};

class Shader
{
private:
    unsigned int ID;
    std::vector<ShaderType> bit_width;
public:

    Shader(const char* vertexPath, const char* fragmentPath);
    
    void use();
    // utility uniform functions
    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;
    void setVector2(const std::string &name, float x, float y) const;

    int bitWidth();
    size_t bitSize(const size_t index);
};
  
#endif