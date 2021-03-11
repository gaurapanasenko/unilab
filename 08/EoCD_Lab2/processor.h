#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <cstring>
#include "gui/texture.h"
#include "julia.h"

class Processor {
    Julia jul;
    shared_ptr<const ImageData> data;
    const Texture texture;
public:
    Processor(shared_ptr<const ImageData> input);
    void update();

    bool process_image(const char *name);
};

#endif // PROCESSOR_H
