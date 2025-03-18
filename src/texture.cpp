#include <glad/glad.h>
#include "texture.hpp"

unsigned int texture::loadTexture(const char* path) {}
void texture::bindTexture(unsigned int textureID) {
    glBindTexture(GL_TEXTURE_2D, textureID);
}