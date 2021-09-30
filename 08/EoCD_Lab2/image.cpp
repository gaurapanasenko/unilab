#include <algorithm>
#include <complex>
#include "image.h"

Image::Image(std::shared_ptr<const pixel_t[]> data,
             const int size[])
    : data(data), size{size[0], size[1]}
{
}

Image Image::julia(const int size[], const Julia &jul)
{
    const int width = size[0], height = size[1];
    std::shared_ptr<pixel_t[]> out_data(new pixel_t[size[0] * size[1]]);
    std::pair<float, float> coord;
    std::complex<float> z(jul.getZ()[0], jul.getZ()[1]), c;
    int maxIters = jul.getMaxIterations();
    float inf = jul.getInfinityBorder();
    int i, j, k, cur = 0;
    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            coord = jul.getCoords(i, j, size);
            cur = 0;
            c = std::complex<float>(jul.getZ()[0], jul.getZ()[1]);
            z = std::complex<float>(coord.first, coord.second);
            for (k = 0; k < maxIters; k++) {
                z = pow(z, 2) + c;
                if (abs(z) > inf) {
                    cur = 46 * log(1+k * 255 / maxIters);
                    break;
                }
            }
            out_data[j * width + i][0] = 0 * i / width;
            out_data[j * width + i][1] = cur;
            out_data[j * width + i][2] = 0 * j / height;
            /*for (k = 0; k < 3; k++)
                out_data[j * width + i][k] = cur;*/
            out_data[j * width + i][3] = 255;
        }
    }
    return {out_data, size};
}
