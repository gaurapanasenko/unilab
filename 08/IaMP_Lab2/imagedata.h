#ifndef IMAGEDATA_H
#define IMAGEDATA_H
#include "image.h"

class ImageData {
public:
    shared_ptr<const Image> image;
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

#endif // IMAGEDATA_H
