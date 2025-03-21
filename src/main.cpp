// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context
// creation, etc.)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include <stdio.h>

#include <iostream>
#include <memory>
#include <stdexcept>
#include "fstream"
//可执行文件路径查找
#include "PathSystem.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#define GL_SILENCE_DEPRECATION

#include <GLFW/glfw3.h>  // Will drag system OpenGL headers

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and
// compatibility with old VS compilers. To link with VS2010-era libraries, VS2015+ requires linking with
// legacy_stdio_definitions.lib, which we do using this pragma. Your own project should not be affected, as you are
// likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

//添加json管理
#include "jsonManager.h"
//添加窗口头
#include "createItems.h"
#include "itemManager.h"
#include "glfwloader.h"
#include "setCycle.h"
#include "showByDay.h"
#include "showByItem.h"
#include "todayTarget.h"


// Main code
int main(int, char**)
{
    //--------------------------------获取程序可执行文件路径路径-----------------------------------
    auto pathSystem = std::make_shared<PathSystem>();
    std::string exeDir = pathSystem->getExecutableDir();
    if (exeDir.empty()) {
        std::cerr << "无法获取可执行文件目录" << std::endl;
        return 1;
    }
    //std::cout << "可执行文件目录: " << exeDir << std::endl;
    //----------------------------------------------------------------------------------------




    auto GlfwLoader = std::make_shared<glfwloader>();
    GlfwLoader->glfwInitialize();
    GlfwLoader->glfwCreatWindow();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;    // Enable Multi-Viewport / Platform Windows
    // io.ConfigViewportsNoAutoMerge = true;
    // io.ConfigViewportsNoTaskBarIcon = true;




    //设置imgui.ini文件的路径----------------------------------------------------------
    // 获取可执行文件的路径
    // 设置 ImGui 的配置文件路径
    std::string imguiInitPath=exeDir+"imgui.ini";
    std::cout<<imguiInitPath<<std::endl;
    io.IniFilename = imguiInitPath.c_str();



    
    
    ////设置风格///////////////////////////////////////////////////////////////////////////////////////////
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
    // 可选：设置全局圆角（需要 ImGui 1.84+）
    style.WindowRounding = 5.0f;
    style.ChildRounding = 5.0f;
    style.FrameRounding = 5.0f;
    style.PopupRounding = 5.0f;
    style.ScrollbarRounding = 5.0f;
    style.GrabRounding = 5.0f;
    // ImGui::SliderFloat("WindowRounding", &style.WindowRounding, 0.0f, 12.0f, "%.0f");
    // ImGui::SliderFloat("ChildRounding", &style.ChildRounding, 0.0f, 12.0f, "%.0f");
    // ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f");
    // ImGui::SliderFloat("PopupRounding", &style.PopupRounding, 0.0f, 12.0f, "%.0f");
    // ImGui::SliderFloat("ScrollbarRounding", &style.ScrollbarRounding, 0.0f, 12.0f, "%.0f");
    // ImGui::SliderFloat("GrabRounding", &style.GrabRounding, 0.0f, 12.0f, "%.0f");
    ///////////////////////////////////////////////////////////////////////////////////////////////
    
    
    
    
    
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(GlfwLoader->getWindowPtr(), true);
    #ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
    #endif
    ImGui_ImplOpenGL3_Init(GlfwLoader->getGLSLversion());
    
    
    //设置imgui渲染字体-----------------------------------------------------------------
std::string fontPath = exeDir + "font\\SourceHanSansCN-Regular.ttf";  // 换成包含简体中文的字体
std::ifstream file(fontPath);
if (!file) {
    std::cerr << "字体文件未找到: " << fontPath << std::endl;
}

std::cout << "尝试加载字体: " << fontPath << std::endl;

ImFont* font = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 40.0f, nullptr,
                                            io.Fonts->GetGlyphRangesChineseFull());
if (font == nullptr) {
    std::cerr << "字体加载失败！" << std::endl;
} else {
    std::cout << "字体加载成功！" << std::endl;
}

io.Fonts->Build();  // 确保 ImGui 刷新字体缓存


    // Our state
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


    ////创建资源对象///////////////////////////////////////////////////////////////////////////////////
    auto JsonManager = std::make_shared<jsonManager>();
    JsonManager->getExePath(pathSystem);
    //创建json管理员
    //JsonManager->setCycle({2, 4, 6, 8, 10});
    //JsonManager->addEvent("学习 C++ 基础");
    //初始化窗口对象
    todayTarget TodayTarget(JsonManager);
    showByDay ShowByDay(JsonManager);
    showByItem ShowByItem(JsonManager);
    createItem CreateItem(JsonManager);
    itemManager ItemManager(JsonManager);
    setCycle SetCycle(JsonManager);

    ///////////////////////////////////////////////////////////////////////////////////////






    // 主循环♻️
#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the
    // imgui.ini file. You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!glfwWindowShouldClose(GlfwLoader->getWindowPtr()))
#endif
    {
        //判断窗口最小化活动
        if (GlfwLoader->OnAttachMinimizeWindow(ImGui_ImplGlfw_Sleep))
        {
            continue;
        }
        else
        {
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();



        //必须在主循环中设置整个页面的dock///////////////////////////////////////////////////////
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
        
        
        /////开始绘制窗口/////////////////////////////////////////////////////////////////////

        CreateItem.showWindow();
        ItemManager.showWindow();
        SetCycle.showWindow();
        ShowByItem.showWindow();
        ShowByDay.showWindow();
        TodayTarget.showWindow();


        //////////////////////////////////////////////////////////////////////////


        ImGui::Render();
        GlfwLoader->clearWindow(clear_color);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

        GlfwLoader->SwapFrame();
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    GlfwLoader->DestoryGLFW();

    std::cin.get();
    return 0;
}
