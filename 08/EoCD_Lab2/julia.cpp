#include <cstring>
#include <imgui.h>
#include "julia.h"

Julia::Julia()
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
    return changed;
}
