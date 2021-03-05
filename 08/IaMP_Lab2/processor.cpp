#include "processor.h"

Processor::Processor(shared_ptr<const ImageData> input)
    : orig(input), data(input) {
    updateDissection();
}

void Processor::updateDissection() {
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
