#include <string>
#include <memory>
#include <sstream>
#include <map>
#include "app.h"
#include "image.h"

using namespace std;

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

int main(int, char**)
{
    App app;
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
            //images.push_back(ImagePack(file_dialog->selected_path));
            string path(file_dialog->selected_path);
            auto pathImage = std::make_shared<ImageFile>(path);
            auto gray = std::make_shared<ImageData>(convert_to_rgb(*pathImage));
            {
                stringstream ss;
                ss << "Original Image: " << file_dialog->selected_fn
                   << " " << file_dialog->selected_path;
                auto data = std::make_shared<ImageData>(pathImage);
                images.insert(make_pair(ss.str(), make_shared<Processor>(data)));
            }
            {
                stringstream ss;
                ss << "Gray Image: " << file_dialog->selected_fn
                   << " " << file_dialog->selected_path;
                images.insert(make_pair(ss.str(), make_shared<Processor>(gray)));
            }
            {
                stringstream ss;
                ss << "Equalization: " << file_dialog->selected_fn
                   << " " << file_dialog->selected_path;
                auto data = std::make_shared<ImageData>(equalize_gray(gray));
                images.insert(make_pair(ss.str(), make_shared<Processor>(data)));
            }
        }

        for (auto image = images.begin(); image != images.end();) {
            bool opened = true;
            const char * name = image->first.c_str();
            auto item = image->second;
            auto data = item->data;
            auto img = data->image;
            auto texture = (ImTextureID)(intptr_t)(item->texture.id);
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
            changed |= ImGui::Checkbox("Dissected", &item->dissected);
            if (item->dissected) {
                changed |= ImGui::DragIntRange2(
                            "dissection x", item->dissection_x,
                            item->dissection_x + 1, 1, 0, 256, "Min: %d", "Max: %d");
                changed |= ImGui::DragFloatRange2(
                            "dissection y", item->dissection_y,
                            item->dissection_y + 1, 0.01, 0, 1, "Min: %.2f", "Max: %.2f");
                ImGui::PlotLines("Lines", item->dissection, 256, 0, NULL, 0, 1.0f, ImVec2(0, 80.0f));
            }
            if (changed) item->updateDissection();
            ImGui::End();

            ImGui::PopID();
            if (!opened) {
                image = images.erase(image);
            } else {
                ++image;
            }
        }
        app.end_loop();
    }

    return 0;
}
