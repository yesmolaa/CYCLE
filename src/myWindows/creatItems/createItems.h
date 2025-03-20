#pragma once
#include <cstring>
#include "imgui.h"
#include "iostream"
#include "jsonManager.h"
#include "memory"


class createItem
{
private:
    std::shared_ptr<jsonManager> m_jsonManagerSptr;
public:
    createItem(std::shared_ptr<jsonManager> jsonManagerSharedPtr) { this->m_jsonManagerSptr = jsonManagerSharedPtr; }

    void pushItem(const char* charVec)
    {
        if (strlen(charVec)==0)
        {
            //std::cout<<"empty test:p"<<charVec<<"p"<<std::endl;
        }
        else
        {
            //std::cout<<"empty test:p"<<charVec<<"p"<<std::endl;
            std::string pushString(charVec);
            //std::cout<<"empty test:p"<<pushString<<"p"<<std::endl;
            m_jsonManagerSptr->addEvent(pushString);
        }
    }


    void showWindow()
    {
        ImGui::Begin("创建待复习");
        // Note: we are using a fixed-sized buffer for simplicity here. See ImGuiInputTextFlags_CallbackResize
        // and the code in misc/cpp/imgui_stdlib.h for how to setup InputText() for dynamically resizing strings.
        static char text[1024 * 16] = "添加一个新的复习计划吧！推荐格式：\n标题\n内容";
        static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
        // ImGui::CheckboxFlags("ImGuiInputTextFlags_ReadOnly", &flags, ImGuiInputTextFlags_ReadOnly);
        // ImGui::CheckboxFlags("ImGuiInputTextFlags_AllowTabInput", &flags, ImGuiInputTextFlags_AllowTabInput);
        // ImGui::CheckboxFlags("ImGuiInputTextFlags_CtrlEnterForNewLine",
        // &flags,ImGuiInputTextFlags_CtrlEnterForNewLine);
        ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text),
                                  ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 15), flags);
        if (ImGui::Button("提交"))
        {
            pushItem(text);
            text [ 0 ] = '\0' ;
        }
        ImGui::SameLine();
        if (ImGui::Button("清除"))
        {
            text [ 0 ] = '\0' ;
        }
        ImGui::End();
    }
};
