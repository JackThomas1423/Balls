#include "shader.hpp"

shader::VertexLayout shader::parseVertexShaderCode(const char* code) {
    std::vector<std::string> types;
    std::vector<unsigned int> sizes;
    std::string str(code);
    std::regex layoutRegex(LayoutRegexPattern);
    std::sregex_iterator chunk_start = std::sregex_iterator(str.begin(), str.end(), layoutRegex);
    std::sregex_iterator chunk_end = std::sregex_iterator();

    for (std::sregex_iterator i = chunk_start; i != chunk_end; ++i) {
        std::smatch chunk = *i;
        std::string size = chunk[3].str();
        types.push_back(chunk[1].str());
        sizes.push_back(size.empty() ? 1 : std::stoi(size));
    }
    return VertexLayout(types, sizes);
}

unsigned int shader::compileShaderCode(const char* code, GLenum type)
{
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &code, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        return -1;
    }

    return shader;
}

unsigned int shader::createShaderProgram(const char* vertexCode, const char* fragmentCode)
{
    unsigned int vertexShader = compileShaderCode(vertexCode, GL_VERTEX_SHADER);
    unsigned int fragmentShader = compileShaderCode(fragmentCode, GL_FRAGMENT_SHADER);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return -1;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

std::string shader::getShaderCode(const char* path)
{
    std::ifstream file;
    file.open(path);
    if (!file.is_open())
    {
        std::cout << "ERROR::SHADER::FILE_NOT_FOUND\n";
        return "";
    }

    std::string code;
    std::string line;
    while (std::getline(file, line))
    {
        code += line + "\n";
    }

    file.close();
    return code;
}