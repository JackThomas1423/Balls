#include <glad/glad.h>
#include "stb_image.h"

namespace texture {
    unsigned int loadTexture(const char* path);
    void bindTexture(unsigned int textureID);
    void unbindTexture();
    void deleteTexture(unsigned int textureID);
};