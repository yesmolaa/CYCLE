#pragma once
#include "imgui.h"
#include "jsonManager.h"
#include "memory"

class showByDay
{
private:
    std::string readonly_text= "还没有计划";
    std::shared_ptr<jsonManager> m_jsonManagerSptr;

public:
    showByDay(std::shared_ptr<jsonManager> jsonManagerSharedPtr)
    {
        this->m_jsonManagerSptr=jsonManagerSharedPtr;
        GetContentByDay();
    }

    void GetContentByDay() {
        std::string getstring=m_jsonManagerSptr->showEventByTime();
        if(getstring.empty())
        {
            readonly_text="还没有计划";
        }
        else {
            readonly_text=getstring;
        }
    }

    void showWindow()
    {
        ImGui::Begin("将要进行的计划");
        // Note: we are using a fixed-sized buffer for simplicity here. See ImGuiInputTextFlags_CallbackResize
        // and the code in misc/cpp/imgui_stdlib.h for how to setup InputText() for dynamically resizing strings.

        // 刷新用窗口
        if (ImGui::Button("刷新"))
        {
            GetContentByDay();
        }
        static ImGuiInputTextFlags flags = ImGuiInputTextFlags_ReadOnly;
        // ImGui::CheckboxFlags("ImGuiInputTextFlags_ReadOnly", &flags, ImGuiInputTextFlags_ReadOnly);
        // ImGui::CheckboxFlags("ImGuiInputTextFlags_AllowTabInput", &flags, ImGuiInputTextFlags_AllowTabInput);
        // ImGui::CheckboxFlags("ImGuiInputTextFlags_CtrlEnterForNewLine",
        // &flags,ImGuiInputTextFlags_CtrlEnterForNewLine); ImGui::InputTextMultiline("##source", readonly_text,
        // IM_ARRAYSIZE(text),ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), flags);
        //  推荐方法：直接使用 ImGui::TextDisabled 显示只读文本
        ImGui :: PushStyleColor ( ImGuiCol_TextDisabled , ImVec4 ( 0.0f , 0.0f , 0.0f , 0.9f ) );
        ImGui ::TextDisabled("%s", readonly_text.c_str());
        ImGui::PopStyleColor();
        ImGui::End();
    }
};
