#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "image.h"

Image::Image(std::shared_ptr<const pixel_t[]> data,
             const int width, const int height)
    : data(data), width(width), height(height)
{
}

Image Image::fromFile(const char *path)
{
    int width = 0, height = 0;
    pixel_t *data = (pixel_t *)stbi_load(path, &width, &height, NULL, COMP);
    assert(data != NULL);
    std::shared_ptr<pixel_t[]> ptr(data, stbi_image_free);
    return {ptr, width, height};
}


std::shared_ptr<const Image> convert_to_gray(const Image &image)
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

shared_ptr<const Image> dissect(const Image &image, float dissection[])
{
    const int size = image.width * image.height;
    std::shared_ptr<pixel_t[]> data(new pixel_t[size]);
    const pixel_t *in_data = image.data.get();
    color_t s[256];

    for (int i = 0; i < 256; i++) {
        s[i] = dissection[i] * i;
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < 3; j++) {
            data[i][j] = s[in_data[i][j]];
        }
        data[i][3] = 255;
    }

    return std::make_shared<Image>(data, image.width, image.height);
}
