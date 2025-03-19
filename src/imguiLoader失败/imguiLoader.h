#pragma once
#include <memory>

#include "glfwloader.h"
#include "imgui.h"

class imguiLoader
{
private:
    std::shared_ptr<glfwloader> m_glfwLoader;
    ImGuiIO& io;

public:
    // 构造函数，通过注入 ImGui::GetIO() 的引用来初始化 io
    explicit imguiLoader(std::shared_ptr<glfwloader> glfwLoader) : m_glfwLoader(glfwLoader), io(ImGui::GetIO()) {}
    void IOconfig();
    ImGuiIO& getIO();
    void getGLFWobj(std::shared_ptr<glfwloader>);
    ImVec4 getWindowState();
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