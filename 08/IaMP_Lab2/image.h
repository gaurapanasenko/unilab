#ifndef IMAGE_H
#define IMAGE_H
#include <memory>

/**
 * Size of pixel in bypes.
 *
 * Name means "components" and was taken from `stb_image.h` header file library.
 */
#define COMP 4

using std::shared_ptr;

/**
 * @brief Channel type for image.
 * Used `unsigned char` because our channel values are in range [0, 255]
 * so has 8-bit per channel.
 */
typedef unsigned char channel_t;

/**
 * @brief Pixel type.
 * One pixel has `COMP` channels and channel defined by `channel_t` type.
 * Used 8-bit chanel with values that in range [0, 255].
 * Pixel has 4 channels (red, green, blue and alpha) so takes 4 bytes.
 */
typedef channel_t pixel_t[COMP];

class Image
{
public:
    /**
     * @brief Constant pixels array.
     * Stores smart pointer to `height * width` pixels of an image.
     */
    shared_ptr<const pixel_t[]> data;
    /**
     * @brief Width of an image.
     */
    const int width;
    /**
     * @brief Height of an image.
     */
    const int height;

    /**
     * @brief Image construcor.
     * Stores reference to pixels, height and width of an image.
     * @param data Smart reference to array of pixels,
     * size must be `height * width`.
     * @param width Width of an image.
     * @param height Height of an image.
     */
    Image(shared_ptr<const pixel_t[]> data,
          const int width, const int height);
    /**
     * @brief Read image from file.
     * @param path Path of image file.
     * @return `Image` class, that also references to pixels.
     */
    static Image fromFile(const char *path);

    /**
     * @brief Calculates histogram of an image.
     * Calculates histogram by red channel so Image must be grayscaled before.
     * @return Smart pointer to histogram.
     */
    shared_ptr<const int[256]> calcHistogram() const;

    /**
     * @brief Convert image to grayscale.
     * Still uses 24 bytes per pixel.
     * @return Smart pointer to grayscaled image.
     */
    shared_ptr<const Image> toGray() const;

    /**
     * @brief Dissect image.
     * Dissecting image of every color channel except of alpha.
     * For best result Image must be grayscaled before.
     * @param dissection New values for all possible channel values.
     * @return Smart pointer to dissected image.
     */
    shared_ptr<const Image> dissect(channel_t dissection[256]) const;

    /**
     * @brief Dilate image.
     * Applies max filter. For this filter used mask with size
     * `2*params[0]+1`x`2*params[1]+1`
     * @param params size of mask.
     * @return Smart pointer to dilated image.
     */
    shared_ptr<const Image> dilate(int params[2]) const;
};


#endif // IMAGE_H
