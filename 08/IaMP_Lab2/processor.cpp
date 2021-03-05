#include "processor.h"

Processor::Processor(shared_ptr<const ImageData> input)
    : orig(input), data(input) {
    updateDissection();
}
