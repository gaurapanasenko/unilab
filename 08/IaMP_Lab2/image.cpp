#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <algorithm>
#include "image.h"

using std::min;
using std::max;

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

std::shared_ptr<const int[256]> Image::calcHistogram() const
{
    std::shared_ptr<int[256]> histogram(new int[256]{0});
    const pixel_t *in_data = data.get();

    int size = height * width;
    for (int i = 0; i < size; i++)
        histogram[in_data[i][0]]++;

    return histogram;
}

std::shared_ptr<const Image> Image::toGray() const
{
    int size = width * height;
    std::shared_ptr<pixel_t[]> out_data(new pixel_t[size]);
    const pixel_t *in_data = data.get(), *pixel;
    unsigned char avg;

    for (int i = 0; i < size; i++) {
        pixel = &in_data[i];
        avg = (*pixel[0] + *pixel[1] + *pixel[2]) / 3;
        //avg = fmax((int)pixel[0], (int)pixel[1]);
        //avg = fmax((int)avg, (int)pixel[2]);
        for (int k = 0; k < 3; k++) {
            out_data[i][k] = avg;
        }
        out_data[i][3] = 255;
    }

    return std::make_shared<Image>(out_data, width, height);
}

shared_ptr<const Image> Image::dissect(channel_t dissection[]) const
{
    const int size = width * height;
    std::shared_ptr<pixel_t[]> out_data(new pixel_t[size]);
    const pixel_t *in_data = data.get();

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < 3; j++) {
            out_data[i][j] = dissection[in_data[i][j]];
        }
        out_data[i][3] = 255;
    }

    return std::make_shared<Image>(out_data, width, height);
}

shared_ptr<const Image> Image::dilate(int params[2]) const
{
    std::shared_ptr<pixel_t[]> out_data(new pixel_t[width * height]);
    const pixel_t *in_data = data.get();
    int i, j, k, l, kbegin, kend, lbegin, lend;

    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            channel_t mx = in_data[i * width + j][0];
            kbegin = max(i - params[1], 0);
            kend = min(i + params[1] + 1, height);
            lbegin = max(j - params[0], 0);
            lend = min(j + params[0], width - 1);
            for (k = kbegin; k != kend; k++) {
                for (l = lbegin; l != lend; l++) {
                    mx = max(mx, in_data[k * width + l][0]);
                }
            }
            for (k = 0; k < 3; k++) {
                out_data[i * width + j][k] = mx;
            }
            out_data[i * width + j][3] = 255;
        }
    }
    return std::make_shared<Image>(out_data, width, height);
}

shared_ptr<const Image> Image::erode(int params[]) const
{
    std::shared_ptr<pixel_t[]> out_data(new pixel_t[width * height]);
    const pixel_t *in_data = data.get();
    int i, j, k, l, kbegin, kend, lbegin, lend;

    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            channel_t mx = in_data[i * width + j][0];
            kbegin = max(i - params[1], 0);
            kend = min(i + params[1] + 1, height);
            lbegin = max(j - params[0], 0);
            lend = min(j + params[0], width - 1);
            for (k = kbegin; k != kend; k++) {
                for (l = lbegin; l != lend; l++) {
                    mx = min(mx, in_data[k * width + l][0]);
                }
            }
            for (k = 0; k < 3; k++) {
                out_data[i * width + j][k] = mx;
            }
            out_data[i * width + j][3] = 255;
        }
    }
    return std::make_shared<Image>(out_data, width, height);
}
