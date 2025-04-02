#include "texture.hpp"

unsigned int texture::loadTexture(const char* path, GLenum drawMethod, GLenum pixelFormat) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, drawMethod);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, drawMethod);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, pixelFormat);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, pixelFormat);

    // Load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 1) ? GL_RED : (nrChannels == 3) ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture: " << path << std::endl;
    }
    stbi_image_free(data);

    return textureID;
}

void texture::bindTexture(unsigned int textureID) {
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void texture::unbindTexture() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture::deleteTexture(unsigned int textureID) {
    glDeleteTextures(1, &textureID);
}