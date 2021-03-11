#ifndef APP_H
#define APP_H
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include "ImGuiFileBrowser.h"
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

class App
{
private:
    const std::string name;
    GLFWwindow* window;

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    imgui_addons::ImGuiFileBrowser file_dialog;

    bool show_demo_window = false;
public:
    App(const std::string& name);
    ~App();
    void begin_loop();
    void end_loop();
    bool should_closed();
    imgui_addons::ImGuiFileBrowser *getFile_dialog();
private:

    void showMainMenu();
};

#endif // APP_H
