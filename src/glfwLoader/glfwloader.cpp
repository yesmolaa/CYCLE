#include "glfwloader.h"
#include<stdio.h>

//判断GLFW环境是否可行
static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

bool glfwloader::glfwInitialize()
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) return false;

        // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100 (WebGL 1.0)
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    return true;
#elif defined(IMGUI_IMPL_OPENGL_ES3)
    // GL ES 3.0 + GLSL 300 es (WebGL 2.0)
    const char* glsl_version = "#version 300 es";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    return true;
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
    return true;
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
    return true;
#endif
}

bool glfwloader::glfwCreatWindow()
{
    // Create window with graphics context
    this->windowPrt = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
    if (this->windowPrt == nullptr) return false;
    glfwMakeContextCurrent(this->windowPrt);
    glfwSwapInterval(1);  // Enable vsync
    return true;
}

GLFWwindow* glfwloader::getWindowPtr() { return this->windowPrt; }

int glfwloader::glfwWindowCloseRoNot() { return glfwWindowShouldClose(this->windowPrt); }

const char* glfwloader::getGLSLversion()
{
    return this->glsl_version;
}


bool glfwloader::OnAttachMinimizeWindow(std::function<void(int)> ImguiSleep)
{
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your
    // inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite
    // your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or
    // clear/overwrite your copy of the keyboard data. Generally you may always pass all inputs to dear imgui, and hide
    // them from your application based on those two flags.
    glfwPollEvents();
    if (glfwGetWindowAttrib(this->windowPrt, GLFW_ICONIFIED) != 0)
    {
        ImguiSleep(10);
        return true;
    }
    else
    {
        return false;
    }
}

void glfwloader::clearWindow(ImVec4 clear_color)
{
    // Rendering
    int display_w, display_h;
    glfwGetFramebufferSize(this->windowPrt, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
}

void glfwloader::SwapFrame()
{
    glfwSwapBuffers(this->windowPrt);
}

void glfwloader::DestoryGLFW() {
    glfwDestroyWindow(this->windowPrt);
    glfwTerminate();
}
