#ifndef IMAGE_H
#define IMAGE_H
#include <memory>

#define COMP 4

using namespace std;

typedef unsigned char color_t;
typedef color_t pixel_t[COMP];

class Image
{
public:
    shared_ptr<const pixel_t[]> data;
    const int width;
    const int height;

    Image(shared_ptr<const pixel_t[]> data,
          const int width, const int height);
    static Image fromFile(const char *path);
};

shared_ptr<const Image> convert_to_gray(const Image &image);
shared_ptr<const Image> dissect(const Image &image, float dissection[256]);

#endif // IMAGE_H
