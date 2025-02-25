#include "glsl_basics.hpp"

#include <vector>
#include <regex>

namespace glsl_handler {

std::vector<std::smatch> applyRegex(const std::string& code, const std::string& regex);

glsl_basics::ShaderType stringToShaderType(const std::string& str);
int shaderTypeToSize(glsl_basics::ShaderType type);

}