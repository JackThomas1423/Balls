#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <array>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
  

class Shader
{
private:
    unsigned int ID;
    std::vector<int> bit_width;
public:

    Shader(const char* vertexPath, const char* fragmentPath);
    
    void use();
    // utility uniform functions
    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;
    void setVector2(const std::string &name, float x, float y) const;

    int bitWidth();
    int bitSize(const size_t index);
};
  
#endif