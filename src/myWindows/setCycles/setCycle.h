#pragma once
#include "imgui.h"
#include "vector"
#include "string"
class setCycle
{
private:
    struct Column
    {
        int id;         // 栏目唯一标识
        float width;    // 栏目宽度
        float height;   // 栏目高度
        ImColor color;  // 栏目颜色
    };
    // 静态 vector，用于存储每个输入框的值，初始化为0
    
    public:
    setCycle()
    {
        static std::vector<int> values;
        ImGui::Begin("设置复习周期");
        ImGui::PushItemWidth(100);
        
        // 动态获取 n 的值，这里假设 n 是从某处获取的，例如用户输入或其他逻辑
        static int n=5;  // 示例值，您可以动态设置 n
        // 确保 vector 的大小至少为 n
        if (values.size() < static_cast<size_t>(n))
        {
            // 调整 vector 大小，并用默认值（如0）初始化新元素
            values.resize(n, 0);
        }

        // 动态创建输入框
        for (int i = 0; i < n; i++)
        {
            // 为每个输入框创建唯一的标识符，避免 ImGui 认为它们是相同的控件
            std::string label = "第 " + std::to_string(i + 1) + " 次复习##input" + std::to_string(i);
            ImGui::InputInt(label.c_str(), &values[i]);
        }
        // 设置输入框宽度为100像素
        // 如果需要动态添加或删除输入框，可以在这里添加相应的逻辑
        // 例如，添加一个按钮来增加一个新的输入框
        if (ImGui::Button("增加一次复习"))
        {
            values.push_back(0);  // 添加一个新的值，默认为0
            n++;
        }

        // 删除最后一个输入框的按钮
        if (n > 1 && ImGui::Button("删除最后一次复习"))
        {
            values.pop_back();  // 移除最后一个值
            n--;                // 更新 n 的值
        }
        ImGui::PopItemWidth();

        ImGui::End();
    }
};