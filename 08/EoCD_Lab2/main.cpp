#include <string>
#include <memory>
#include <sstream>

#include "gui/app.h"
#include "gui/texture.h"
#include "julia.h"
#include "image.h"

#define PROJECT_NAME "IaMP_Lab2"

using namespace std;

int main(int, char**)
{
    App app(PROJECT_NAME);
    Julia jul;
    shared_ptr<const Image> image;
    const Texture texture;
    int size[2] = {200, 200};
    auto tex_id = (void*)(intptr_t)(texture.id);
    bool changed = true;
    image = make_shared<Image>(Image::julia(size, jul));
    texture.update(*image);

    // Main loop
    while (!app.should_closed())
    {
        app.begin_loop();

        ImVec2 sz(image->size[0], image->size[1]);
        ImGui::SetNextWindowSize(sz, ImGuiCond_FirstUseEver);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("Julia");
        ImVec2 cont_sz = ImGui::GetContentRegionAvail();
        ImVec2 img_size(cont_sz.x, image->size[1] * cont_sz.x / image->size[0]);
        ImGui::Image(tex_id, img_size);
        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::Begin("Julia Config", NULL, ImGuiWindowFlags_AlwaysAutoResize);
        changed = jul.imConfig();
        changed |= ImGui::SliderInt2("Size", size, 0, 500);
        if (changed) {
            image = make_shared<Image>(Image::julia(size, jul));
            texture.update(*image);
        }
        ImGui::End();

        app.end_loop();
    }

    return 0;
}
