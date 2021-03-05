#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <cstring>
#include "imagedata.h"
#include "gui/texture.h"

class Processor {
public:
    shared_ptr<const ImageData> orig;
    shared_ptr<const ImageData> data;
    const Texture texture;
    int dissection_x[2] = {100, 200};
    float dissection_y[2] = {0, 1};
    float dissection[256] = {0};
    bool dissected;
    Processor(shared_ptr<const ImageData> input);
    void updateDissection();
};

#endif // PROCESSOR_H
