#include "glsl_basics.hpp"

#include <vector>
#include <regex>

namespace glsl_handler {

template <typename T>
std::vector<T> applyRexex(const std::string& code, const std::string& regex);

glsl_basics::ShaderType stringToShaderType(const std::string& type);

}