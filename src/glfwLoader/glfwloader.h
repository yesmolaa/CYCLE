// 注意，该glfw封装的api

#pragma once

#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <functional>
#include "imgui.h"



class glfwloader
{
private:
    //窗口指针
    GLFWwindow* windowPrt;
    //GLSL版本
    const char* glsl_version = "#version 130";
    
public:
    //设置窗口配置
    bool glfwInitialize();
    //创建窗口
    bool glfwCreatWindow();
    //获取窗口指针
    GLFWwindow* getWindowPtr();
    //获取GLSL版本
    const char* getGLSLversion();
    //判断窗口是否关闭，关闭返回1，未关闭返回0
    int glfwWindowCloseRoNot();
    //glfw检查窗口最小化活动
    bool OnAttachMinimizeWindow(std::function<void(int)> ImguiSleep);
    //glfw清除屏幕工作
    void clearWindow(ImVec4 clear_color);
    //循环的最后交换帧缓冲
    void SwapFrame();
    //清除窗口
    void DestoryGLFW();
};