#pragma once
#include <iostream>
#include <memory>
#include "string"
#include "imgui.h"
#include "jsonManager.h"

class todayTarget
{
private:
    std::string readonly_text;
    std::shared_ptr<jsonManager> m_jsonManagerSptr;

public:
    // 传入json对象指针以访问对象
    todayTarget(std::shared_ptr<jsonManager> jsonManagerSharedPtr) { 

        this->m_jsonManagerSptr = jsonManagerSharedPtr; 
        GetTodayContent();
    }

    void GetTodayContent() {
        std::string getstring=m_jsonManagerSptr->TodayReview();
        if(getstring.empty())
        {
            readonly_text="今天没有复习内容";
        }
        else {
            readonly_text=getstring;
        }
        std::cout<<"打印C字符串"<<readonly_text<<std::endl;
    }

    void showWindow()
    {
        ImGui::Begin("今日复习");
        // Note: we are using a fixed-sized buffer for simplicity here. See ImGuiInputTextFlags_CallbackResize
        // and the code in misc/cpp/imgui_stdlib.h for how to setup InputText() for dynamically resizing strings.

        static ImGuiInputTextFlags flags = ImGuiInputTextFlags_ReadOnly;
        // ImGui::CheckboxFlags("ImGuiInputTextFlags_ReadOnly", &flags, ImGuiInputTextFlags_ReadOnly);
        // ImGui::CheckboxFlags("ImGuiInputTextFlags_AllowTabInput", &flags, ImGuiInputTextFlags_AllowTabInput);
        // ImGui::CheckboxFlags("ImGuiInputTextFlags_CtrlEnterForNewLine",
        // &flags,ImGuiInputTextFlags_CtrlEnterForNewLine); ImGui::InputTextMultiline("##source", readonly_text,
        // IM_ARRAYSIZE(text),ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), flags);
        //  推荐方法：直接使用 ImGui::TextDisabled 显示只读文本

        // 保存当前的颜色样式 
        ImGui :: PushStyleColor ( ImGuiCol_TextDisabled , ImVec4 ( 0.0f , 0.0f , 0.0f , 0.9f ) );
        ImGui ::TextDisabled("%s", readonly_text.c_str());
        // 恢复之前的颜色样式
        ImGui::PopStyleColor();
        ImGui::End();
    }
};
