#pragma once
#include <nlohmann/json.hpp>
#include "string"


class jsonManager
{
private:
    nlohmann::json config;  // 存储 config.json
    nlohmann::json event;   // 存储 event.json
    nlohmann::json item;    //存储复习计划信息
    std::string configPath = "config.json";
    std::string eventPath = "event.json";
    std::string itemPath = "item.json";

    // 获取当前日期字符串，格式为 "YYYY-MM-DD"
    std::string getCurrentDate();

    // 加载 JSON 文件到内存
    void loadFile(const std::string &path, nlohmann::json &data);

    // 保存 JSON 数据到文件
    void saveFile(const std::string &path, const nlohmann::json &data);
public:

    // 构造函数，加载 JSON 文件
    jsonManager();

    // 加载 config.json 和 event.json
    void LoadJSON();

    //设置item.json的随机数种子，用于生成每个item的ID
    void setRandomSeed();

    // 获取今天日期并记录在 config.json 中
    std::string getTime();

    // 设置复习循环
    void setCycle(const std::vector<int> &cycle);

    // 添加一个复习项
    void addEvent(const std::string &content);

    void deleteEvent(const std::string &id);
    // 按日期输出所有复习项
    std::string showEventByTime();

    // 按 Item 输出所有复习项
    std::string showEventByItem();

    // 获取今天的复习项
    std::string TodayReview();

    // 释放资源，将修改写回 JSON
    void Free();
};