#include <string>
#include <memory>
#include <sstream>
#include <map>

#include "gui/app.h"
#include "processor.h"

#define PROJECT_NAME "IaMP_Lab2"

using namespace std;

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
            auto gray = make_shared<ImageData>(pathImage->toGray());
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
                auto data = make_shared<ImageData>(gray->equalize());
                auto proc = make_shared<Processor>(data);
                images.insert(make_pair(ss.str(), proc));
            }
        }

        for (auto image = images.begin(); image != images.end();) {
            if (!image->second->process_image(image->first.c_str())) {
                image = images.erase(image);
            } else {
                ++image;
            }
        }
        app.end_loop();
    }

    return 0;
}
