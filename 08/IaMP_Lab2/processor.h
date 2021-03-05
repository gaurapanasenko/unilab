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
    Processor(shared_ptr<const ImageData> input)
        : orig(input), data(input) {
        updateDissection();
    }
    void updateDissection() {
        if (dissected) {
            float diffY = dissection_y[1] - dissection_y[0];
            int diffX = dissection_x[1] - dissection_x[0];
            float diff = diffY / diffX;
            float acc = dissection_y[0];
            memset(dissection, 0, 256 * sizeof(float));
            for (int i = dissection_x[0]; i < dissection_x[1]; i++) {
                acc += diff;
                dissection[i] = acc;
            }
            data = make_shared<ImageData>(dissect(*orig->image, dissection));
        } else
            data = orig;
        texture.update(*data->image);
    }
};

#endif // PROCESSOR_H
