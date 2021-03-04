#ifndef IMAGE_H
#define IMAGE_H
#include "common.h"
#include <string>
#include <memory>
#include <vector>
#include <array>

#define COMP 4

using namespace std;

typedef unsigned char color_t;
typedef color_t pixel_t[COMP];
typedef basic_string<color_t> pixel_buffer;

class Image
{
public:
    shared_ptr<const pixel_t[]> data;
    const int width;
    const int height;

    Image(shared_ptr<const pixel_t[]> data,
          const int width, const int height);
    static Image fromFile(const char *path);
};

class ImageFile : public Image {
public:
    const string path;

    ImageFile(const string& path);
};

class Texture {
public:
    const GLuint id;

    Texture(const Image& image);
    ~Texture();

    static GLuint create(const Image &image);
private:
    Texture(const Texture&);
};

class ImageData {
public:
    shared_ptr<const Image> image;
    const Texture texture;
    shared_ptr<const int[256]> histogramI;
    int maxHistogramI;
    shared_ptr<const float[256]> histogramF;
    float maxHistogramF;

    ImageData(shared_ptr<const Image> image);
    static shared_ptr<const int[256]> calcHistogram(const Image& image);
    static shared_ptr<const float[256]>
    copyHistogram(shared_ptr<const int[256]> histogramI);
};

shared_ptr<const Image>
equalize_gray(shared_ptr<const ImageData> imageData);

shared_ptr<const Image> convert_to_rgb(const Image &image);

#endif // IMAGE_H
