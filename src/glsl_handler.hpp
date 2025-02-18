#include "glsl_basics.hpp"

#include <vector>
#include <regex>

namespace glsl_handler {

std::vector<std::smatch> applyRexex(const std::string& code, const std::string& regex);

glsl_basics::ShaderType stringToShaderType(const std::string& type);

}