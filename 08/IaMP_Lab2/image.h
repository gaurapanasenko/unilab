#ifndef IMAGE_H
#define IMAGE_H
#include "common.h"
#include <string>
#include <memory>
#include <vector>

#define COMP 4

typedef unsigned char color_t;
typedef color_t pixel_t[COMP];
typedef std::basic_string<color_t> pixel_buffer;

class Image
{
private:
    const pixel_buffer data;
    const int width;
    const int height;
public:
    Image(const pixel_buffer& data, const int width, const int height);
    static Image fromFile(const char *path);
    int getWidth() const;
    int getHeight() const;
    const pixel_t * getData() const;
};

class ImageFile : public Image {
    const std::string path;
public:
    ImageFile(const std::string& path);
    const std::string &getPath() const;
};

class Texture {
    const GLuint id;
    std::shared_ptr<const Image> image;
    Texture(const Texture&);
public:
    Texture(std::shared_ptr<const Image> image);
    ~Texture();
    GLuint getId() const;
private:
    GLuint create(const Image &image) const;
};

class ImageData {
    std::shared_ptr<const Image> image;
    const Texture texture;
    int histogramI[256], maxHistogramI;
    float histogramF[256], maxHistogramF;
public:
    ImageData(std::shared_ptr<const Image> image);
    std::shared_ptr<const Image> getImage() const;
    const Texture& getTexture() const;
    int getMaxHistogramI() const;
    float getMaxHistogramF() const;
    const int* getHistogramI() const;
    const float* getHistogramF() const;
};

std::shared_ptr<const Image> equalize_gray(std::shared_ptr<const ImageData> imageData);

std::shared_ptr<const Image> convert_to_rgb(const Image &image);

#endif // IMAGE_H
