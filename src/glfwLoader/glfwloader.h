#pragma once
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

class glfwloader
{
private:
    //窗口指针
    GLFWwindow* windowPrt;
public:
    //设置窗口配置
    bool glfwInit();
    //创建窗口
    void glfwCreatWindow();
    //获取窗口指针
    GLFWwindow* getWindowPtr();
    //判断窗口是否关闭，关闭返回1，未关闭返回0
    int glfwWindowCloseRoNot();
    //glfw需要在事件循环中重复执行的代码
    void OnAttach();
    //循环的最后交换帧缓冲
    void SwapFrame();
    //清除窗口
    void DestoryGLFW();
};