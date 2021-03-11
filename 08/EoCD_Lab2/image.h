#ifndef IMAGE_H
#define IMAGE_H
#include <memory>
#import "julia.h"

#define COMP 4

using namespace std;

typedef unsigned char color_t;
typedef color_t pixel_t[COMP];

class Image
{
public:
    shared_ptr<const pixel_t[]> data;
    const int size[2];

    Image(shared_ptr<const pixel_t[]> data, const int size[2]);
    static Image julia(const int size[2], const Julia& jul);
};


#endif // IMAGE_H
