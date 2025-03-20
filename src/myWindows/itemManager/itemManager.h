#pragma once
#include "iostream"
#include "imgui.h"
#include "vector"
#include "string"
#include "jsonManager.h"
#include <sstream>


class itemManager
{
private:
    std::shared_ptr<jsonManager> m_jsonManagerSptr;
    const char* windowName="管理复习项目";
    int deleteID=0;
public:
    itemManager(std::shared_ptr<jsonManager> jsonManagerSharedPtr)
    {
        this->m_jsonManagerSptr = jsonManagerSharedPtr;   
    }

    void showWindow()
    {

        ImGui::Begin(windowName);
        // 为每个输入框创建唯一的标识符，避免 ImGui 认为它们是相同的控件
        std::string label = "输入ID进行删除##input";
        //ImGui::InputInt(label.c_str(), &jsonCycle[i]);
        if (ImGui::InputInt(label.c_str(), &deleteID))
        {
        }


        if(ImGui::Button("删除"))
        {
            //std::cout<<"删除的ID是："<<deleteID<<std::endl;
            m_jsonManagerSptr->deleteEvent(std::to_string(deleteID));
        }


        ImGui::End();
    }


};