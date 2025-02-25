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

    //interpret vertex shader structs
    for (std::smatch chunk : glsl_handler::applyRegex(vertexCode,glsl_basics::struct_regex)) {
        std::string struct_name = chunk[1];
        std::string contents = chunk[2];

        std::vector<glsl_basics::ShaderInput> inputs;
        for (std::smatch part : glsl_handler::applyRegex(contents,glsl_basics::struct_part_regex)) {
            glsl_basics::ShaderType type = glsl_handler::stringToShaderType(part[1].str());
            std::optional<int> size = (!part[3].str().empty()) ? std::optional<int>{std::stoi(part[3].str())} : std::nullopt;
            inputs.push_back(glsl_basics::ShaderInput(type,part[2].str(),size,1));
        }
        structs.push_back(glsl_basics::ShaderStruct(struct_name,inputs));
    }
    for (auto x : structs) {
        std::cout << x.name << std::endl;
        for (auto y : x.inputs) {
            std::cout << y.name << std::endl;
            std::cout << (int)y.type << std::endl;
        }
    }
    exit(-1);
    
    //interpret vertex shader code
    /*
    std::regex lrp(glsl_basics::layout_regex);
    std::sregex_iterator chunk_start = std::sregex_iterator(vertexCode.begin(), vertexCode.end(), lrp);
    std::sregex_iterator chunk_end = std::sregex_iterator();

    std::string modifiedCode = vertexCode;
    std::ptrdiff_t offset = 0;

    int location = 0;
    for (std::sregex_iterator i = chunk_start; i != chunk_end; ++i) {
        std::smatch match = *i;
        glsl_basics::ShaderType type = glsl_handler::stringToShaderType(match[1].str());
        std::string modified = "layout (location = " + std::to_string(location) + ") " ;
        modifiedCode.insert(match.position() + offset, modified);
        types.push_back(type);
        location += type.offset;
        offset += modified.length();
    }
    vertexCode = modifiedCode;
    */

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