#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <cstring>
#include "imagedata.h"
#include "gui/texture.h"

class Processor {
    shared_ptr<const ImageData> orig;
    shared_ptr<const ImageData> data;
    const Texture texture;
    int dissection_x[2] = {100, 200};
    float dissection_y[2] = {0, 1};
    float dissection[256] = {0};
    int dilate_params[2] = {1, 1};
    bool dissected = false, dilate = false;
public:
    Processor(shared_ptr<const ImageData> input);
    void updateDissection();

    bool process_image(const char *name);
};

#endif // PROCESSOR_H
