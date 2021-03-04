#include <string>
#include <memory>
#include <sstream>
#include <map>
#include "app.h"
#include "image.h"

using namespace std;

int main(int, char**)
{
    App app;
    map<string, shared_ptr<const ImageData>> images;

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
                images.insert(make_pair(ss.str(), data));
            }
            {
                stringstream ss;
                ss << "Gray Image: " << file_dialog->selected_fn
                   << " " << file_dialog->selected_path;
                images.insert(make_pair(ss.str(), gray));
            }
            {
                stringstream ss;
                ss << "Equalization: " << file_dialog->selected_fn
                   << " " << file_dialog->selected_path;
                auto data = std::make_shared<ImageData>(equalize_gray(gray));
                images.insert(make_pair(ss.str(), data));
            }
        }

        for (auto image = images.begin(); image != images.end();) {
            bool opened = true;
            const char * name = image->first.c_str();
            auto item = image->second;
            auto img = item->image;
            auto texture = (ImTextureID)(intptr_t)(item->texture.id);
            ImGui::PushID(name);

            ImVec2 size(img->width, img->height);
            ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
            ImGui::Begin(name, &opened);
            ImGui::Image(texture, ImGui::GetContentRegionAvail());
            ImGui::End();
            ImGui::PopStyleVar();

            char info_name[128];
            snprintf(info_name, 128, "Info %s", name);
            ImGui::Begin(info_name);
            ImGui::Text("pointer = %ld", (intptr_t)texture);
            ImGui::Text("size = %d x %d", img->width, img->height);
            ImGui::PlotHistogram(
                        "##", item->histogramF.get(), 256, 0, "Histogram", 0.0f,
                        item->maxHistogramF, ImVec2(0, 100.0f));
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
