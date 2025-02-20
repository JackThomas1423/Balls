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

    location_sizes.push_back(0);
    std::ptrdiff_t offset = 0;
    for (std::smatch match : glsl_handler::applyRexex(vertexCode,glsl_basics::vertex_regex)) {
        std::string modified = "layout(location = " + std::to_string(location_sizes.back()) + ") ";
        vertexCode.insert(match.position() + offset,modified);
        location_sizes.push_back((match[3].str().empty()) ? location_sizes.back() + 1 : location_sizes.back() + std::stoi(match[3].str()));
        offset += modified.length();
    }

    //interpret structs for vertex shader

    for (std::smatch chunk : glsl_handler::applyRexex(vertexCode,glsl_basics::struct_regex)) {
        std::string struct_name = chunk[1];
        std::string contents = chunk[2];

        glsl_basics::ShaderStruct shaderStruct;
        shaderStruct.name = struct_name;
        for (std::smatch part : glsl_handler::applyRexex(contents,glsl_basics::struct_part_regex)) {
            std::string n = part[3].str();
            std::optional<int> size = (!n.empty()) ? std::optional<int>{std::stoi(n)} : std::nullopt;
            glsl_basics::ShaderStructType type = glsl_basics::ShaderStructType(glsl_handler::stringToShaderType(part[1].str()),part[2].str(),size);
            shaderStruct.types.push_back(type);            
        }
    }

    //compile shaders
    
    const char* vShaderCode = vertexCode.c_str();
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

inline int Shader::getLocationOffset(int index) const {
    return location_sizes.at(index);
}

inline int Shader::getLocationLength() const {
    return location_sizes.size();
}