#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <algorithm>

void LoadTextureFromFile(const pixel_t* data, int width, int height,
                         GLuint* out_texture)
{
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);

    *out_texture = image_texture;
}

Image::Image(std::shared_ptr<const pixel_t[]> data,
             const int width, const int height)
    : data(data), width(width), height(height)
{
}

Image Image::fromFile(const char *path)
{
    int width = 0, height = 0;
    pixel_t *data = (pixel_t *)stbi_load(path, &width, &height, NULL, COMP);
    IM_ASSERT(data != NULL);
    std::shared_ptr<pixel_t[]> ptr(data);
    return {ptr, width, height};
}

ImageFile::ImageFile(const std::string &path)
    : Image(fromFile(path.c_str())), path(path) {}


std::shared_ptr<const Image> convert_to_rgb(const Image &image)
{
    int width = image.width, height = image.height;
    std::shared_ptr<pixel_t[]> data(new pixel_t[width * height]);
    const pixel_t *in_data = image.data.get(), *pixel;
    unsigned char avg;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            pixel = &in_data[i * width + j];
            avg = (*pixel[0] + *pixel[1] + *pixel[2]) / 3;
            //avg = fmax((int)pixel[0], (int)pixel[1]);
            //avg = fmax((int)avg, (int)pixel[2]);
            for (int k = 0; k < 3; k++) {
                data[i * width + j][k] = avg;
            }
            data[i * width + j][3] = 255;
        }
    }

    return std::make_shared<Image>(data, width, height);
}

std::shared_ptr<const Image> equalize_gray(std::shared_ptr<const ImageData> imageData)
{
    std::shared_ptr<const Image> image = imageData->image;
    int width = image->width, height = image->height;
    int size = width * height;
    std::shared_ptr<pixel_t[]> data(new pixel_t[width * height]);
    const pixel_t *in_data = image->data.get();
    const int *histogram = imageData->histogramI.get();
    int accum = 0;
    int s[256];

    for (int i = 0; i < 256; i++) {
        accum += histogram[i];
        s[i] = 255 * accum / size;
    }

    for (int i = 0; i < size; i++) {
        color_t cur = s[in_data[i][0]];
        data[i][0] = cur;
        data[i][1] = cur;
        data[i][2] = cur;
        data[i][3] = 255;
    }

    return std::make_shared<Image>(data, width, height);
}

Texture::Texture(const Image &image)
    : id(create(image))
{
}

Texture::~Texture()
{
    glDeleteTextures(1, &id);
}

GLuint Texture::create(const Image& image)
{
    GLuint id = 0;
    LoadTextureFromFile(image.data.get(), image.width, image.height, &id);
    return id;
}

ImageData::ImageData(std::shared_ptr<const Image> image)
    : image(image), texture(*image), histogramI(calcHistogram(*image)),
      maxHistogramI(*std::max_element(histogramI.get(), histogramI.get() + 256)),
      histogramF(copyHistogram(histogramI)),
      maxHistogramF(maxHistogramI)
{
}

std::shared_ptr<const int[256]> ImageData::calcHistogram(const Image &image)
{
    const int width = image.width, height = image.height;
    std::shared_ptr<int[256]> histogram(new int[256]{0});
    const pixel_t *data = image.data.get();

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            histogram[data[i * width+j][0]]++;

    return histogram;
}

std::shared_ptr<const float[256]>
ImageData::copyHistogram(std::shared_ptr<const int[256]> histogramI)
{
    std::shared_ptr<float[256]> histogram(new float[256]{0});
    std::copy(histogramI.get(), histogramI.get() + 256, histogram.get());
    return histogram;
}
