#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <array>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <numeric>

#include "glsl_basics.hpp"
#include "glsl_handler.hpp"

class Shader
{
private:
    unsigned int ID;
    std::vector<int> location_sizes;
public:

    Shader(const char* vertexPath, const char* fragmentPath);
    
    void use();
    // utility uniform functions
    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;
    void setVector2(const std::string &name, float x, float y) const;

    inline int getLocationOffset(int index) const;
    inline int getLocationLength() const;
};
  
#endif