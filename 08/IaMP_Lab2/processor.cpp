#include <stdio.h>
#include "processor.h"
#include "imgui.h"

Processor::Processor(shared_ptr<const ImageData> input)
    : orig(input), data(input) {
    updateDissection();
}

void Processor::updateDissection() {
    data = orig;
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
        data = make_shared<ImageData>(data->image->dissect(dissection));
    }
    if (dilate) {
        data = make_shared<ImageData>(data->image->dilate(dilate_params));
    }
    texture.update(*data->image);
}

bool Processor::process_image(const char *name) {
    bool opened = true;
    auto img = data->image;
    auto tex_id = (void*)(intptr_t)(texture.id);
    ImGui::PushID(name);

    ImVec2 size(img->width, img->height);
    ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
    ImGui::Begin(name, &opened, ImGuiWindowFlags_NoSavedSettings);
    ImGui::Image(tex_id, ImGui::GetContentRegionAvail());
    ImGui::End();
    ImGui::PopStyleVar();

    char info_name[128];
    snprintf(info_name, 128, "Info %s", name);
    ImGui::Begin(info_name, NULL, ImGuiWindowFlags_NoSavedSettings);
    ImGui::Text("pointer = %ld", (intptr_t)tex_id);
    ImGui::Text("size = %d x %d", img->width, img->height);
    ImGui::PlotHistogram(
                "##", data->histogramF.get(), 256, 0, "Histogram", 0.0f,
                data->maxHistogramF, ImVec2(0, 100.0f));
    bool changed = false;
    ImGui::Spacing();
    changed |= ImGui::Checkbox("Dissected", &dissected);
    if (dissected) {
        {
            auto val = dissection_x;
            changed |= ImGui::DragIntRange2("dissection x", val, val + 1, 1, 0,
                                            256, "Min: %d", "Max: %d");
        }
        {
            auto val = dissection_y;
            changed |= ImGui::DragFloatRange2("dissection y", val, val + 1, 0.01,
                                              0, 1, "Min: %.2f", "Max: %.2f");
        }
        ImGui::PlotLines("Lines", dissection, 256, 0, NULL, 0, 1.0f,
                         ImVec2(0, 80.0f));
    }
    ImGui::Spacing();
    changed |= ImGui::Checkbox("Dilate", &dilate);
    if (dilate) {
        changed |= ImGui::SliderInt2("dilate params", dilate_params, 0, 16);
    }
    if (changed) updateDissection();
    ImGui::End();

    ImGui::PopID();
    return opened;
}
