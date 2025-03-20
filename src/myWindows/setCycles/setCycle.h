#pragma once
#include <cstddef>
#include "iostream"
#include "imgui.h"
#include "vector"
#include "string"
#include "jsonManager.h"


class setCycle
{
private:
    std::shared_ptr<jsonManager> m_jsonManagerSptr;
    //获取json中存储的循环信息
    std::vector<int> jsonCycle;

    //存储每次复习的时间
    std::vector<std::string> reviewTime;
    
public:
    setCycle(std::shared_ptr<jsonManager> jsonManagerSharedPtr)
    {
        this->m_jsonManagerSptr = jsonManagerSharedPtr;
        this->jsonCycle=jsonManagerSharedPtr->getCycle();
        //记录初始化的循环
        //previousJsonCycle = jsonCycle;
        for(int i=0;i<jsonCycle.size();i++)
        {
            std::cout<<"测试输出循环"<<jsonCycle[i]<<std::endl;
        }
        
        SetReviewTime();
        //std::cout<<n<<std::endl;
    }

    //更新复习日期
    void SetReviewTime()
    {
        std::string today=m_jsonManagerSptr->getCurrentDate();
        struct tm tmevent = {};
        strptime(today.c_str(), "%Y-%m-%d", &tmevent);  // 解析日期字符串
        reviewTime.resize(jsonCycle.size());//注意，resize和reserve的区别，此处不能使用reserve
        for(size_t i=0;i<jsonCycle.size();i++)
        {
            int days = jsonCycle[i];  // 获取复习间隔天数
            tmevent.tm_mday += days;  // 加上天数
            mktime(&tmevent);         // 标准化时间结构体（会自动处理月份和年份的变化）
            char buffer[11];
            strftime(buffer, sizeof(buffer), "%Y-%m-%d", &tmevent);  // 格式化为 "YYYY-MM-DD"
            reviewTime[i]=buffer;
            std::cout<<"第"<<i<<"次复习时间为"<<reviewTime[i]<<std::endl;
        }
        //std::cout<<"---------------------测试-----------------------"<<std::endl;
    }

    void updateCycle()
    {
        //将设置好的时间同步回json文件
        m_jsonManagerSptr->setCycle(jsonCycle);
        //同时更新界面上的复习时间
        SetReviewTime();
    }



    void showWindow()
    {
        
        ImGui::Begin("设置复习周期");
        ImGui::PushItemWidth(150);
        
        // 动态获取 n 的值，这里假设 n 是从某处获取的，例如用户输入或其他逻辑

        static int n=jsonCycle.size();  // 示例值，您可以动态设置 n
        // 确保 vector 的大小至少为 n
        // if (jsonCycle.size() < static_cast<size_t>(n))
        // {
        //     // 调整 vector 大小，并用默认值（如0）初始化新元素
        //     jsonCycle.resize(n, 0);
        // }

        // 动态创建输入框
        for (int i = 0; i < n; i++)
        {
            // 为每个输入框创建唯一的标识符，避免 ImGui 认为它们是相同的控件
            std::string label = "第 " + std::to_string(i + 1) + " 次复习" +"在："+reviewTime[i]+"##input";
            //ImGui::InputInt(label.c_str(), &jsonCycle[i]);
            if (ImGui::InputInt(label.c_str(), &jsonCycle[i])) {
                // 如果输入控件失去焦点且值被提交，ImGui::InputInt 返回 true
                std::cout<<"发生一次修改"<<std::endl;
                if(jsonCycle[i]>=1)
                {
                    //std::cout<<"执行io"<<std::endl;
                    updateCycle();
                }
            }
            //确保每个复习计划隔1天
            if(jsonCycle[i]<1)
            {
                jsonCycle[i]=1;
            }
        }
        // 设置输入框宽度为100像素
        // 如果需要动态添加或删除输入框，可以在这里添加相应的逻辑
        // 例如，添加一个按钮来增加一个新的输入框
        if (ImGui::Button("增加一次复习"))
        {
            jsonCycle.push_back(1);  // 添加一个新的值，默认为0
            updateCycle();
            n++;
        }

        // 删除最后一个输入框的按钮
        if (n > 1 && ImGui::Button("删除最后一次复习"))
        {
            jsonCycle.pop_back();  // 移除最后一个值
            updateCycle();
            n--;                // 更新 n 的值
        }

        ImGui::PopItemWidth();

        ImGui::End();
    }
};