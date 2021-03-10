#ifndef IMAGEDATA_H
#define IMAGEDATA_H
#include "image.h"

/**
 * @brief Image metadata class.
 * Stores pointer to image, and histograms.
 * Also stores maximum value of histogram.
 */
class ImageData {
public:
    /**
     * @brief Smart pointer to image.
     */
    shared_ptr<const Image> image;
    /**
     * @brief Smart pointer to histogram with integer type.
     */
    shared_ptr<const int[256]> histogramI;
    /**
     * @brief Maximum value of `histogramI`.
     */
    int maxHistogramI;
    /**
     * @brief Smart pointer to histogram with float type.
     */
    shared_ptr<const float[256]> histogramF;
    /**
     * @brief Maximum value of `histogramF`.
     */
    float maxHistogramF;

    /**
     * @brief ImageData constructor.
     * Calulates and stores histogram based on `image`.
     * @param image Smart pointer to image.
     */
    ImageData(shared_ptr<const Image> image);

    /**
     * @brief Converts histogram from integer type to float.
     * @param histogramI Integer type histogram.
     * @return Float type histogram.
     */
    static shared_ptr<const float[256]>
    copyHistogram(shared_ptr<const int[256]> histogramI);

    /**
     * @brief Equalize image using histogram.
     * @return New equalized image.
     */
    shared_ptr<const Image> equalize() const;
};

#endif // IMAGEDATA_H
