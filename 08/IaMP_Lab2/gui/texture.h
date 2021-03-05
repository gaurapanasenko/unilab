#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/gl.h>
#include "../image.h"

class Texture {
public:
    const GLuint id;

    Texture();
    void update(const Image& image) const;
    ~Texture();
private:
    static GLuint create();

    Texture(const Texture&);
};

#endif // TEXTURE_H
