#include "texture.h"

void LoadTextureFromFile(const pixel_t* data, int width, int height,
                         GLuint id)
{
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
}


Texture::Texture() : id(create()) {}

void Texture::update(const Image &image) const
{
    LoadTextureFromFile(image.data.get(), image.width, image.height, id);
}

Texture::~Texture()
{
    glDeleteTextures(1, &id);
}

GLuint Texture::create()
{
    GLuint id = 0;
    glGenTextures(1, &id);
    return id;
}
