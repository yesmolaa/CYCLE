#pragma once
#include <memory>

#include "glfwloader.h"

#include "imgui.h"

class imguiLoader
{
private:
    std::shared_ptr<glfwloader> m_glfwLoader;
    ImGuiIO& io;
    struct windowState
    {
        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    }windowState;

public:
    void IOconfig();
    void getGLFWobj(std::shared_ptr<glfwloader>);
    void setStyle();
    void setPlatformRendererBackend();
    void satrtImguiFrame();
    //从此开始编写窗口逻辑
    void showDemoWindow();
    void showMyWindow();
    void renderFlag();
    void Draw();
    void sortMultiWindow();
    void DestoryImgui();
};