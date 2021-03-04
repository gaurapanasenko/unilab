#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <algorithm>

void LoadTextureFromFile(unsigned char* data, int width, int height, GLuint* out_texture)
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    *out_texture = image_texture;
}

int Image::getWidth() const
{
    return width;
}

int Image::getHeight() const
{
    return height;
}

const pixel_t *Image::getData() const
{
    return (pixel_t *)data.c_str();
}

Image::Image(const pixel_buffer &data, const int width, const int height)
    : data(data), width(width), height(height)
{
}

Image Image::fromFile(const char *path)
{
    int width, height;
    pixel_t *data = (pixel_t *)stbi_load(path, &width, &height, NULL, COMP);
    IM_ASSERT(data != NULL);
    int size = height * width * COMP;
    pixel_buffer buffer((color_t*)data, size);
    stbi_image_free(data);
    return {buffer, width, height};
}

ImageFile::ImageFile(const std::string &path)
    : Image(fromFile(path.c_str())), path(path) {}

const std::string& ImageFile::getPath() const
{
    return path;
}

std::shared_ptr<const Image> convert_to_rgb(const Image &image)
{
    int width = image.getWidth(), height = image.getHeight();
    pixel_buffer vec_data(width * height * COMP, 0);
    pixel_t *data = (pixel_t*)(&vec_data[0]);
    const pixel_t *in_data = image.getData(), *pixel;
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

    return std::make_shared<Image>(vec_data, image.getWidth(), image.getHeight());
}

std::shared_ptr<const Image> equalize_gray(std::shared_ptr<const ImageData> imageData)
{
    std::shared_ptr<const Image> image = imageData->getImage();
    int width = image->getWidth(), height = image->getHeight();
    int size = width * height;
    pixel_buffer vec_data(size * COMP, 0);
    pixel_t *data = (pixel_t*)(&vec_data[0]);
    const pixel_t *in_data = image->getData();
    const int *histogram = imageData->getHistogramI();
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

    return std::make_shared<Image>(vec_data, width, height);
}

Texture::Texture(std::shared_ptr<const Image> image) : id(create(*image)), image(image)
{
}

Texture::~Texture()
{
    glDeleteTextures(1, &id);
}

GLuint Texture::getId() const
{
    return id;
}

GLuint Texture::create(const Image& image) const
{
    GLuint id;
    LoadTextureFromFile((unsigned char *)image.getData(), image.getWidth(),
                        image.getHeight(), &id);
    return id;
}

std::shared_ptr<const Image> ImageData::getImage() const
{
    return image;
}

const Texture &ImageData::getTexture() const
{
    return texture;
}

int ImageData::getMaxHistogramI() const
{
    return maxHistogramI;
}

float ImageData::getMaxHistogramF() const
{
    return maxHistogramF;
}

const int *ImageData::getHistogramI() const
{
    return histogramI;
}

const float *ImageData::getHistogramF() const
{
    return histogramF;
}

ImageData::ImageData(std::shared_ptr<const Image> image) : image(image), texture(image)
{

    const int width = image->getWidth(), height = image->getHeight();
    const pixel_t *data = image->getData();

    memset(histogramI, 0, 256 * sizeof(int));
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            histogramI[data[i * width+j][0]]++;

    maxHistogramI = histogramI[0];
    for (int i = 0; i < 256; i++) {
        histogramF[i] = histogramI[i];
        maxHistogramI = std::max(maxHistogramI, histogramI[i]);
    }
    maxHistogramF = maxHistogramI;
}
