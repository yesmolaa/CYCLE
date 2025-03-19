#include "imguiLoader.h"

#include <memory>

#include "glfwloader.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void imguiLoader::IOconfig()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    this->io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;    // Enable Multi-Viewport / Platform Windows
    // io.ConfigViewportsNoAutoMerge = true;
    // io.ConfigViewportsNoTaskBarIcon = true;
}

void imguiLoader::getGLFWobj(std::shared_ptr<glfwloader> o_glfwloader) { this->m_glfwLoader = o_glfwloader; }

void imguiLoader::setStyle()
{
    // Setup Dear ImGui style
    // ImGui::StyleColorsDark();
    ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular
    // ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
}

void imguiLoader::setPlatformRendererBackend()
{
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(this->m_glfwLoader->getWindowPtr(), true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
#endif
    ImGui_ImplOpenGL3_Init(this->m_glfwLoader->getGLSLversion());
}

void imguiLoader::satrtImguiFrame()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void imguiLoader::showDemoWindow()
{
    if (this->windowState.show_demo_window) ImGui::ShowDemoWindow(&this->windowState.show_demo_window);
}

void imguiLoader::showMyWindow()
{
    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");  // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");  // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window",
                        &this->windowState.show_demo_window);  // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &this->windowState.show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);  // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&this->windowState.clear_color);  // Edit 3 floats representing a color

        if (ImGui::Button(
                "Button"))  // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }
    // 3. Show another simple window.
    if (this->windowState.show_another_window)
    {
        ImGui::Begin(
            "Another Window",
            &this->windowState.show_another_window);  // Pass a pointer to our bool variable (the window will have a
                                                      // closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me")) this->windowState.show_another_window = false;
        ImGui::End();
    }
}

void imguiLoader::renderFlag() { ImGui::Render(); }

void imguiLoader::Draw() { ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); }

void imguiLoader::sortMultiWindow()
{
    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste
    // this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void imguiLoader::DestoryImgui()
{  // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
