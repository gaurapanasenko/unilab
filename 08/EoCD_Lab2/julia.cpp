#include <cstring>
#include <imgui.h>
#include "julia.h"

Julia::Julia()
    : minValue{-2, -2}, maxValue{2, 2},
      maxIterations(50), infinityBorder(10), z{0, 0}
{

}

const float *Julia::getMin() const
{
    return minValue;
}

const float *Julia::getMax() const
{
    return maxValue;
}

int Julia::getMaxIterations() const
{
    return maxIterations;
}

float Julia::getInfinityBorder() const
{
    return infinityBorder;
}

const float *Julia::getZ() const {
    return z;
}

std::pair<float, float> Julia::getCoords(int x, int y, const int size[]) const
{
    std::pair<float, float> out(0, 0);
    out.first = (maxValue[0] - minValue[0]) * x / size[0] + minValue[0];
    out.second = (maxValue[1] - minValue[1]) * y / size[1] + minValue[1];
    return out;
}

bool Julia::imConfig()
{
    bool changed = false;
    changed |= ImGui::DragFloatRange2("X", minValue, maxValue+1, 0.01, -3, 3);
    changed |= ImGui::DragFloatRange2("Y", minValue+1, maxValue, 0.01, -3, 3);
    changed |= ImGui::SliderInt("Max iterations", &maxIterations, 1, 400);
    changed |= ImGui::DragFloat2("Z", z, 0.01, -3, 3);
    return changed;
}
