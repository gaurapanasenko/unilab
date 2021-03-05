#include <string>
#include <memory>
#include <sstream>
#include <map>

#include "gui/app.h"
#include "processor.h"

#define PROJECT_NAME "IaMP_Lab2"

using namespace std;

bool process_image(const char *name, Processor& item) {
    bool opened = true;
    auto data = item.data;
    auto img = data->image;
    auto texture = (ImTextureID)(intptr_t)(item.texture.id);
    ImGui::PushID(name);

    ImVec2 size(img->width, img->height);
    ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
    ImGui::Begin(name, &opened, ImGuiWindowFlags_NoSavedSettings);
    ImGui::Image(texture, ImGui::GetContentRegionAvail());
    ImGui::End();
    ImGui::PopStyleVar();

    char info_name[128];
    snprintf(info_name, 128, "Info %s", name);
    ImGui::Begin(info_name, NULL, ImGuiWindowFlags_NoSavedSettings);
    ImGui::Text("pointer = %ld", (intptr_t)texture);
    ImGui::Text("size = %d x %d", img->width, img->height);
    ImGui::PlotHistogram(
                "##", data->histogramF.get(), 256, 0, "Histogram", 0.0f,
                data->maxHistogramF, ImVec2(0, 100.0f));
    bool changed = false;
    changed |= ImGui::Checkbox("Dissected", &item.dissected);
    if (item.dissected) {
        {
            auto val = item.dissection_x;
            changed |= ImGui::DragIntRange2("dissection x", val, val + 1, 1, 0,
                                            256, "Min: %d", "Max: %d");
        }
        {
            auto val = item.dissection_y;
            changed |= ImGui::DragFloatRange2("dissection y", val, val + 1, 0.01,
                                              0, 1, "Min: %.2f", "Max: %.2f");
        }
        ImGui::PlotLines("Lines", item.dissection, 256, 0, NULL, 0, 1.0f,
                         ImVec2(0, 80.0f));
    }
    if (changed) item.updateDissection();
    ImGui::End();

    ImGui::PopID();
    return opened;
}

int main(int, char**)
{
    App app(PROJECT_NAME);
    map<string, shared_ptr<Processor>> images;

    imgui_addons::ImGuiFileBrowser *file_dialog = app.getFile_dialog();
    // Main loop
    while (!app.should_closed())
    {
        app.begin_loop();
        auto mode = imgui_addons::ImGuiFileBrowser::DialogMode::OPEN;

        if (file_dialog->showFileDialog(
                    "Open File", mode, ImVec2(700, 310), ".png,.jpg,.bmp"))
        {
            string path(file_dialog->selected_path);
            auto img = Image::fromFile(path.c_str());
            auto pathImage = make_shared<Image>(img);
            auto gray = make_shared<ImageData>(convert_to_gray(*pathImage));
            {
                stringstream ss;
                ss << "Original Image: " << file_dialog->selected_fn
                   << " " << file_dialog->selected_path;
                auto data = make_shared<ImageData>(pathImage);
                auto proc = make_shared<Processor>(data);
                images.insert(make_pair(ss.str(), proc));
            }
            {
                stringstream ss;
                ss << "Gray Image: " << file_dialog->selected_fn
                   << " " << file_dialog->selected_path;
                auto proc = make_shared<Processor>(gray);
                images.insert(make_pair(ss.str(), proc));
            }
            {
                stringstream ss;
                ss << "Equalization: " << file_dialog->selected_fn
                   << " " << file_dialog->selected_path;
                auto data = make_shared<ImageData>(equalize_gray(gray));
                auto proc = make_shared<Processor>(data);
                images.insert(make_pair(ss.str(), proc));
            }
        }

        for (auto image = images.begin(); image != images.end();) {
            if (!process_image(image->first.c_str(), *image->second)) {
                image = images.erase(image);
            } else {
                ++image;
            }
        }
        app.end_loop();
    }

    return 0;
}
