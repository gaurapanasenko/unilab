#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <cstring>
#include "imagedata.h"
#include "gui/texture.h"

/**
 * @brief Processing image.
 * Calculates histogram, applies dissection or max filter and stores result
 * image to texture.
 *
 * Also have implements interface to manipulate parameters in this class
 * with imgui.
 */
class Processor {
    /**
     * @brief Original image with calculated histogram.
     */
    shared_ptr<const ImageData> orig;
    /**
     * @brief Currently shown image on display.
     */
    shared_ptr<const ImageData> data;
    /**
     * @brief Texture used to draw with OpenGL.
     */
    const Texture texture;
    /**
     * @brief Range of channel value where apply dissection.
     */
    int dissection_x[2];
    /**
     * @brief Range of proportional coefficients between dissection values.
     * Coefficients must be in range [0, 1] to make sure that new channel values
     * will not out of [0, 255] range.
     */
    float dissection_y[2];
    /**
     * @brief New channel values for dissection for image.
     */
    channel_t dissection[256];
    /**
     * @brief Size of max filter mask.
     */
    int dilate_params[2];
    /**
     * @brief Flag to apply dissection to the image.
     */
    bool dissected;
    /**
     * @brief Flag to apply dilation to the image.
     */
    bool dilate;
public:
    /**
     * @brief Processor constuctor.
     * @param input Input image that will be processed by this class.
     */
    Processor(shared_ptr<const ImageData> input);
    /**
     * @brief Calculates new image and stores it to texture.
     */
    void updateDissection();

    /**
     * @brief Implements interface to manipulate fields.
     * @param name name of window that will be shown.
     * @return boolean value that shows that it is needed to update image.
     */
    bool process_image(const char *name);
};

#endif // PROCESSOR_H
