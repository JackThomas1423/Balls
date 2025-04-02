#pragma once

#include <glad/glad.h>
#include <iostream>
#include "stb_image.h"

namespace texture {
    unsigned int loadTexture(const char* path, GLenum drawMethod, GLenum pixelFormat);
    void bindTexture(unsigned int textureID);
    void unbindTexture();
    void deleteTexture(unsigned int textureID);
};