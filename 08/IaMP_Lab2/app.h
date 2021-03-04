#ifndef APP_H
#define APP_H
#include "common.h"

class App
{
private:
    GLFWwindow* window;

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    imgui_addons::ImGuiFileBrowser file_dialog;

    bool show_demo_window = false;
public:
    App();
    ~App();
    void begin_loop();
    void end_loop();
    bool should_closed();
    imgui_addons::ImGuiFileBrowser *getFile_dialog();
private:

    void showMainMenu();
};

#endif // APP_H
