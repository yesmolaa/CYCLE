#include "jsonManager.h"
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <random>
#include <string>
#include <vector>



using json = nlohmann::json;
using namespace std;
// 获取当前日期字符串，格式为 "YYYY-MM-DD"
string jsonManager::getCurrentDate()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    ostringstream oss;
    oss << 1900 + ltm->tm_year << "-" << 1 + ltm->tm_mon << "-" << ltm->tm_mday;
    return oss.str();
}

// 加载 JSON 文件到内存
void jsonManager::loadFile(const string &path, json &data)
{
    ifstream file(path);
    if (file.is_open())
    {
        file >> data;
    }
    else
    {
        data = json::object();  // 如果文件不存在，初始化为空对象
    }
}

// 保存 JSON 数据到文件
void jsonManager::saveFile(const string &path, const json &data)
{
    ofstream file(path);
    if (file.is_open())
    {
        file << data.dump(4);  // 以美观的格式保存
    }
}
// 构造函数，加载 JSON 文件
jsonManager::jsonManager()
{
    LoadJSON();
    setRandomSeed();
}

// 加载 config.json 和 event.json
void jsonManager::LoadJSON()
{
    loadFile(configPath, config);
    loadFile(eventPath, event);
    loadFile(itemPath, item);
}

//设置item.json的随机数种子，用于生成每个item的ID
void jsonManager::setRandomSeed()
{
    string first = "1";
    if (!config.contains("random_seed"))
    {
        config["random_seed"] = first;  //初始化随机数种子为1
    }
    saveFile(configPath, config);
}

// 获取今天日期并记录在 config.json 中
string jsonManager::getTime()
{
    string today = getCurrentDate();
    if (!config.contains("last_updated"))
    {
        config["last_updated"] = today;
    }
    config["last_updated"] = today;  // 更新最后更新时间
    saveFile(configPath, config);
    return today;
}

// 设置复习循环
void jsonManager::setCycle(const vector<int> &cycle)
{
    config["review_cycle"] = cycle;
    saveFile(configPath, config);
}

// 添加一个复习项
void jsonManager::addEvent(const string &content)
{
    string today = getCurrentDate();
    getTime();  // 确保 config.json 的时间是最新的

    // 获取复习周期
    if (!config.contains("review_cycle") || config["review_cycle"].empty())
    {
        cerr << "复习周期未设置！" << endl;
        return;
    }
    const vector<int> &cycle = config["review_cycle"];

    //从item.json获得随机数种子以生成ID
    string randomSeedString = config["random_seed"];
    int randomSeedNum = std::stoi(randomSeedString);
    // 创建一个随机数引擎，例如 Mersenne Twister
    std::mt19937 rng;
    rng.seed(randomSeedNum);
    // 定义一个分布，例如均匀整数分布 int min = 1 ; int max = 100 ;
    std::uniform_int_distribution<int> dist(1, 999999);
    // 生成随机数
    int random_number = dist(rng);
    string id = to_string(random_number);

    //向item.json插入单个复习计划
    // 检查日期是否存在
    struct tm tmitem = {};
    strptime(today.c_str(), "%Y-%m-%d", &tmitem);  // 解析日期字符串
    //我们获得标准化的日期，比如2025-03-04而不是2025-3-4
    char today_std[11];
    strftime(today_std, sizeof(today_std), "%Y-%m-%d", &tmitem);  // 格式化为 "YYYY-MM-DD"
    if (!item.contains(today_std))
    {
        item[today_std] = json::object();  // 如果日期不存在，则创建
    }
    item[today_std][id] = content;
    config["random_seed"] = to_string(randomSeedNum + 1);
    // 保存到 item.json
    saveFile(itemPath, item);

    //向event.json中插入日程
    struct tm tmevent = {};
    strptime(today.c_str(), "%Y-%m-%d", &tmevent);  // 解析日期字符串
    // 在 event.json 中添加复习项
    for (size_t i = 0; i < cycle.size(); ++i)
    {
        int days = cycle[i];  // 获取复习间隔天数
        string date = today;

        // 计算未来的日期
        // 使用简单的日期加减逻辑（假设每个月都是 30 天，仅作示例）
        // 实际项目中建议使用更复杂的日期库（如 Boost.Date_Time 或 C++20 的 <chrono>）
        tmevent.tm_mday += days;  // 加上天数
        mktime(&tmevent);         // 标准化时间结构体（会自动处理月份和年份的变化）
        char buffer[11];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", &tmevent);  // 格式化为 "YYYY-MM-DD"
        date = buffer;

        // 检查日期是否存在
        if (!event.contains(date))
        {
            event[date] = json::object();  // 如果日期不存在，则创建
        }

        // 添加复习项
        event[date][id] = content;
    }

    // 保存到 event.json
    saveFile(eventPath, event);
}

// // 删除指定 ID 的复习项
// void deleteEvent(const string id)
// {
//     for (auto &[date, items] : event.items())
//     {
//         for (auto it = items.begin(); it != items.end();)
//         {
//             if (it.key().find(id) != string::npos)
//             {  // 简单匹配 ID
//                 cout<<it.key()<<endl;
//                 it = items.erase(it);
//             }
//             else
//             {
//                 ++it;
//             }
//         }
//         if (items.empty())
//         {
//             event.erase(date);  // 如果当天没有复习项，删除该日期
//         }
//     }
//     saveFile(eventPath, event);
// }
// 删除指定 ID 的复习项
void jsonManager::deleteEvent(const string &id)
{
    vector<string> datesToRemove;

    // 第一步：遍历并删除匹配的复习项，同时收集需要删除的日期
    for (auto &[date, items] : event.items())
    {
        auto it = items.begin();
        while (it != items.end())  //注意，不能使用for循环删除json数据
        {
            if (it.key().find(id) != string::npos)  // it->first 是键
            {
                cout << it.key() << endl;
                it = items.erase(it);  // 正确地更新迭代器
            }
            else
            {
                ++it;
            }
        }

        // 如果当天没有复习项，记录日期以供后续删除
        if (items.empty())
        {
            datesToRemove.push_back(date);
        }
    }

    // 第二步：删除空的日期项
    for (const auto &date : datesToRemove)
    {
        event.erase(date);
    }

    // 保存文件
    saveFile(eventPath, event);
}

// 按日期输出所有复习项
// string jsonManager::showEventByTime()
// {
//     cout << "按时间输出所有复习项：" << endl;
//     ostringstream oss;
//     for (const auto &[date, items] : event.items())
//     {
//         oss << "日期: " << date << endl;
//         for (const auto &[id, content] : items.items())
//         {
//             oss << "  - " << id << ": " << content << endl;
//         }
//     }
//     return oss.str();
// }

string jsonManager::showEventByTime()
{
    cout << "按时间输出所有复习项：" << endl;
    ostringstream oss;
    for (const auto &[date, items] : event.items())
    {
        oss << "日期: " << date << std::endl;
        for (const auto &[id, content] : items.items())
        {
            // 将 content 中的 \n 替换为 std::endl,防止打印错误
            string formattedContent = content;
            size_t pos = 0;
            while ((pos = formattedContent.find('\n', pos)) != string::npos)
            {
                formattedContent.replace(pos, 1, "\n"); // 这里其实不需要替换，因为 \n 已经是换行符
                pos += 1; // 移动到下一个字符
            }
            oss << "  - " << id << ": " << formattedContent << std::endl;
        }
    }
    return oss.str();
}

// 按 Item 输出所有复习项
string jsonManager::showEventByItem()
{
    cout << "按复习项目输出所有复习项：" << endl;
    ostringstream oss;
    for (const auto &[date, items] : item.items())
    {
        oss << "日期: " << date << endl;
        for (const auto &[id, content] : items.items())
        {
            // 将 content 中的 \n 替换为 std::endl,防止打印错误
            string formattedContent = content;
            size_t pos = 0;
            while ((pos = formattedContent.find('\n', pos)) != string::npos)
            {
                formattedContent.replace(pos, 1, "\n"); // 这里其实不需要替换，因为 \n 已经是换行符
                pos += 1; // 移动到下一个字符
            }
            oss << "  - " << id << ": " << formattedContent << endl;
        }
    }
    return oss.str();
}

// 获取今天的复习项
string jsonManager::TodayReview()
{
    cout << "今天的复习项：" << endl;
    string today = getCurrentDate();
    struct tm tm = {};
    strptime(today.c_str(), "%Y-%m-%d", &tm);  // 解析日期字符串
    //我们获得标准化的日期，比如2025-03-04而不是2025-3-4
    char today_std[11];
    strftime(today_std, sizeof(today_std), "%Y-%m-%d", &tm);  // 格式化为 "YYYY-MM-DD"
    // cout << "今天的日期："<<today_std<< endl;
    if (!event.contains(today_std))
    {
        return "今天没有复习项。";
    }
    ostringstream oss;
    for (const auto &[id, content] : event[today_std].items())
    {
        oss << "  - " << id << ": " << content << endl;
    }
    return oss.str();
}

// 释放资源，将修改写回 JSON
void jsonManager::Free()
{
    saveFile(configPath, config);
    saveFile(eventPath, event);
}