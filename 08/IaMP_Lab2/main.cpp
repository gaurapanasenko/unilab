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

        if (file_dialog->showFileDialog(
                    "Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN,
                    ImVec2(700, 310), ".png,.jpg,.bmp"))
        {
            //images.push_back(ImagePack(file_dialog->selected_path));
            string path(file_dialog->selected_path);
            auto pathImage = std::make_shared<ImageFile>(path);
            auto gray = std::make_shared<ImageData>(convert_to_rgb(*pathImage));
            {
                stringstream ss;
                ss << "Original Image: " << file_dialog->selected_fn << " " << file_dialog->selected_path;
                images.insert(make_pair(ss.str(), std::make_shared<ImageData>(pathImage)));
            }
            {
                stringstream ss;
                ss << "Gray Image: " << file_dialog->selected_fn << " " << file_dialog->selected_path;
                images.insert(make_pair(ss.str(), gray));
            }
            {
                stringstream ss;
                ss << "Equalization: " << file_dialog->selected_fn << " " << file_dialog->selected_path;
                images.insert(make_pair(ss.str(), std::make_shared<ImageData>(equalize_gray(gray))));
            }
        }

        for (auto image = images.begin(); image != images.end();) {
            bool opened = true;
            const char * name = image->first.c_str();
            auto item = image->second;
            ImGui::PushID(name);

            ImVec2 size(item->getImage()->getWidth(), item->getImage()->getHeight());
            ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
            ImGui::Begin(name, &opened);
            ImGui::Image((void*)(intptr_t)(item->getTexture().getId()), ImGui::GetContentRegionAvail());
            ImGui::End();
            ImGui::PopStyleVar();

            char info_name[128];
            snprintf(info_name, 128, "Info %s", name);
            ImGui::Begin(info_name);
            ImGui::Text("pointer = %u", item->getTexture().getId());
            ImGui::Text("size = %d x %d", item->getImage()->getWidth(), item->getImage()->getHeight());
            ImGui::PlotHistogram("##", item->getHistogramF(), 256, 0, "Histogram", 0.0f, item->getMaxHistogramF(), ImVec2(0, 100.0f));
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
