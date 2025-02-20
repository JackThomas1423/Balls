#include "glsl_basics.hpp"

#include <vector>
#include <regex>

namespace glsl_handler {

std::vector<std::smatch> applyRexex(const std::string& code, const std::string& regex);
std::string insertRegex(const std::string& code, const std::string& regex, const std::string& insertText);

glsl_basics::ShaderType stringToShaderType(const std::string& type);

}