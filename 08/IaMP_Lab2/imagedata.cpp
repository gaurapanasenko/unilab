#include <algorithm>
#include "imagedata.h"

ImageData::ImageData(std::shared_ptr<const Image> image)
    : image(image), histogramI(calcHistogram(*image)),
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

std::shared_ptr<const Image>
equalize_gray(std::shared_ptr<const ImageData> imageData)
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
