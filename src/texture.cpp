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
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 4);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture: " << path << std::endl;
    }
    stbi_image_free(data);

    return textureID;
}

void texture::unbindTexture() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture::deleteTexture(unsigned int textureID) {
    glDeleteTextures(1, &textureID);
}