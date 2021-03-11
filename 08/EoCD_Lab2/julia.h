#ifndef JULIA_H
#define JULIA_H
#include "map"

class Julia
{
    float minValue[2] = {-2, -2}, maxValue[2] = {2, 2};
    int maxIterations = 50;
    float infinityBorder = 10;
public:
    Julia();
    const float *getMin() const;
    const float *getMax() const;
    int getMaxIterations() const;
    float getInfinityBorder() const;
    std::pair<float, float> getCoords(int x, int y, const int size[2]) const;
    bool imConfig();
};

#endif // JULIA_H
