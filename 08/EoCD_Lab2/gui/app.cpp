#include <GLFW/glfw3.h>
#include "app.h"

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void App::showMainMenu()
{
    bool open = false;
    if(ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Menu"))
        {
            if (ImGui::MenuItem("Open", NULL))
                open = true;
            if (ImGui::MenuItem("Demo", NULL))
                show_demo_window = true;

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if(open)
        ImGui::OpenPopup("Open File");
}

void App::begin_loop()
{
    glfwPollEvents();

    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    showMainMenu();
}

void App::end_loop()
{
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

    glfwMakeContextCurrent(window);
    glfwSwapBuffers(window);
}

bool App::should_closed()
{
    return glfwWindowShouldClose(window);
}

imgui_addons::ImGuiFileBrowser *App::getFile_dialog()
{
    return &file_dialog;
}

App::App(const std::string& name) : name(name)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        throw "Failed to start glfw.";
    window = glfwCreateWindow(1280, 720, name.c_str(), NULL, NULL);
    if (window == NULL)
        throw "Failed to open window.";
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();
}

App::~App()
{
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}
