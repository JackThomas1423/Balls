#include "shader.hpp"
#include <string>
#include <regex>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    //load vertex and fragment shader code from file
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();		
        vShaderFile.close();
        fShaderFile.close();
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    
    //interpret vertex shader code
    std::stringstream vc(vertexCode);
    std::vector<std::string> vertexCodeLines;
    std::string buffer;
    std::regex vertex_pattern(vertex_regex);
    std::smatch m;

    int line_index = 0;
    std::vector<int> location_sizes = {0};
    while (std::getline(vc, buffer, '\n')) {
        if(std::regex_search(buffer, m, vertex_pattern)) {
            std::string tokens[3];
            int slot = 0;
            for (auto x : m) {
                tokens[slot] = x;
                ++slot;
            }
            
            int last = location_sizes[location_sizes.size() - 1];
            if (!tokens[2].empty()) {
                location_sizes.push_back(last + std::stoi(tokens[2]));
            } else {
                location_sizes.push_back(last + 1);
            }

            if (tokens[1] == "vec2") {
                bit_width.push_back(ShaderType::Vec2);
            } else if (tokens[1] == "vec3") {
                bit_width.push_back(ShaderType::Vec3);
            } else if (tokens[1] == "vec4") {
                bit_width.push_back(ShaderType::Vec4);
            } else if (tokens[1] == "float") {
                bit_width.push_back(ShaderType::Float);
            } else {
                std::cout << "ERROR::SHADER::VERTEX::INTERPRETATION_FAILED" << std::endl;
                std::cout << "ERROR: " << line_index << ": type " << tokens[2] << " not yet supported" << std::endl;
                exit(1);
            }
        }
        vertexCodeLines.push_back(buffer);
        ++line_index;
    }

    std::string newVertexCode = "";

    int location_index = 0;
    for (std::string line : vertexCodeLines) {
        if (std::regex_match(line,m,vertex_pattern)) {
            newVertexCode += "layout(location = " + std::to_string(location_sizes[location_index]) + ") ";
            ++location_index;
        }
        newVertexCode += line + "\n";
    }

    //compile shaders
    
    const char* vShaderCode = newVertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;

    int success;
    char infoLog[512];
   
    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    };
  
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    std::stringstream vc(vertexCode);
    std::string buffer;
    std::regex pattern(vertex_regex);
    std::smatch m;

    int line_index = 0;
    while (std::getline(vc, buffer, '\n')) {
        if(std::regex_search(buffer, m, pattern)) {
            std::string tokens[4];
            int slot = 0;
            for (auto x : m) {
                tokens[slot] = x;
                ++slot;
            }
            std::cout << tokens[3] << std::endl;
            if (tokens[2] == "vec2") {
                bit_width.push_back(ShaderType::Vec2);
            } else if (tokens[2] == "vec3") {
                bit_width.push_back(ShaderType::Vec3);
            } else if (tokens[2] == "vec4") {
                bit_width.push_back(ShaderType::Vec4);
            } else if (tokens[2] == "float") {
                bit_width.push_back(ShaderType::Float);
            } else {
                std::cout << "ERROR::SHADER::VERTEX::INTERPRETATION_FAILED" << std::endl;
                std::cout << "ERROR: " << line_index << ": type " << tokens[2] << " not yet supported" << std::endl;
                exit(1);
            }
        }
        ++line_index;
    }

    // delete the shaders
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() 
{ 
    glUseProgram(ID);
}
void Shader::setBool(const std::string &name, bool value) const
{         
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
}
void Shader::setInt(const std::string &name, int value) const
{ 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}
void Shader::setFloat(const std::string &name, float value) const
{ 
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
}
void Shader::setVector2(const std::string &name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(ID,name.c_str()),x,y);
}

int Shader::bitWidth()
{
    int vertex_width = 0;
    for (const ShaderType& type : bit_width)
        vertex_width += static_cast<int>(type);
    return vertex_width;
}

size_t Shader::bitSize(const size_t index)
{
    return static_cast<size_t>(bit_width[index]);
}