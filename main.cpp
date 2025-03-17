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

class jsonManager
{
private:
    json config;  // 存储 config.json
    json event;   // 存储 event.json
    json item;    //存储复习计划信息
    string configPath = "config.json";
    string eventPath = "event.json";
    string itemPath = "item.json";

    // 获取当前日期字符串，格式为 "YYYY-MM-DD"
    string getCurrentDate()
    {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        ostringstream oss;
        oss << 1900 + ltm->tm_year << "-" << 1 + ltm->tm_mon << "-" << ltm->tm_mday;
        return oss.str();
    }

    // 加载 JSON 文件到内存
    void loadFile(const string &path, json &data)
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
    void saveFile(const string &path, const json &data)
    {
        ofstream file(path);
        if (file.is_open())
        {
            file << data.dump(4);  // 以美观的格式保存
        }
    }

public:
    // 构造函数，加载 JSON 文件
    jsonManager()
    {
        LoadJSON();
        setRandomSeed();
    }

    // 加载 config.json 和 event.json
    void LoadJSON()
    {
        loadFile(configPath, config);
        loadFile(eventPath, event);
        loadFile(itemPath, item);
    }

    //设置item.json的随机数种子，用于生成每个item的ID
    void setRandomSeed()
    {
        string first = "1";
        if (!item.contains("random_seed"))
        {
            item["random_seed"] = first;  //初始化随机数种子为1
        }
        config["random_seed"] = first;
        saveFile(itemPath, item);
    }

    // 获取今天日期并记录在 config.json 中
    string getTime()
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
    void setCycle(const vector<int> &cycle)
    {
        config["review_cycle"] = cycle;
        saveFile(configPath, config);
    }

    // 添加一个复习项
    void addEvent(const string &content)
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
        string randomSeedString = item["random_seed"];
        int randomSeedNum = std::stoi(randomSeedString);
        // 创建一个随机数引擎，例如 Mersenne Twister
        std::mt19937 rng;
        rng.seed(randomSeedNum);
        // 定义一个分布，例如均匀整数分布 int min = 1 ; int max = 100 ;
        std::uniform_int_distribution<int> dist(1, 999999);
        // 生成随机数
        int random_number = dist(rng);
        cout << "测试随机数" << random_number << '\n';
        string id = to_string(random_number);

        //向item.json插入单个复习计划
        // 检查日期是否存在
        if (!item.contains(today))
        {
            item[today] = json::object();  // 如果日期不存在，则创建
        }
        item[today][id]=content;
        item["random_seed"]=to_string(randomSeedNum+1);
        // 保存到 item.json
        saveFile(itemPath, item);

        //向event.json中插入日程
        struct tm tm = {};
        strptime(today.c_str(), "%Y-%m-%d", &tm);  // 解析日期字符串
        // 在 event.json 中添加复习项
        for (size_t i = 0; i < cycle.size(); ++i)
        {
            int days = cycle[i];  // 获取复习间隔天数
            string date = today;

            // 计算未来的日期
            // 使用简单的日期加减逻辑（假设每个月都是 30 天，仅作示例）
            // 实际项目中建议使用更复杂的日期库（如 Boost.Date_Time 或 C++20 的 <chrono>）
            tm.tm_mday += days;  // 加上天数
            mktime(&tm);         // 标准化时间结构体（会自动处理月份和年份的变化）
            char buffer[11];
            strftime(buffer, sizeof(buffer), "%Y-%m-%d", &tm);  // 格式化为 "YYYY-MM-DD"
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

    // 删除指定 ID 的复习项
    void deleteEvent(const string &id)
    {
        for (auto &[date, items] : event.items())
        {
            for (auto it = items.begin(); it != items.end();)
            {
                if (it.key().find(id) != string::npos)
                {  // 简单匹配 ID
                    it = items.erase(it);
                }
                else
                {
                    ++it;
                }
            }
            if (items.empty())
            {
                event.erase(date);  // 如果当天没有复习项，删除该日期
            }
        }
        saveFile(eventPath, event);
    }

    // 按日期输出所有复习项
    string showEventByTime()
    {
        cout << "按时间输出所有复习项：" << endl;
        ostringstream oss;
        for (const auto &[date, items] : event.items())
        {
            oss << "日期: " << date << endl;
            for (const auto &[id, content] : items.items())
            {
                oss << "  - " << id << ": " << content << endl;
            }
        }
        return oss.str();
    }

    // 按 Item 输出所有复习项
    vector<string> showEventByItem()
    {
        vector<string> result;
        for (const auto &[date, items] : event.items())
        {
            for (const auto &[id, content] : items.items())
            {
                result.push_back(content);
            }
        }
        return result;
    }

    // 获取今天的复习项
    string TodayReview()
    {
        cout << "今天的复习项：" << endl;
        string today = getCurrentDate();
        if (!event.contains(today))
        {
            return "今天没有复习项。";
        }
        ostringstream oss;
        for (const auto &[id, content] : event[today].items())
        {
            oss << "  - " << id << ": " << content << endl;
        }
        return oss.str();
    }

    // 释放资源，将修改写回 JSON
    void Free()
    {
        saveFile(configPath, config);
        saveFile(eventPath, event);
    }
};

int main()
{
    jsonManager manager;

    // 设置复习周期
    manager.setCycle({2, 4, 6, 8, 10});

    // 添加复习项
    manager.addEvent("学习 C++ 基础");
    manager.addEvent("复习数据结构");

    // 输出今天的复习项
    cout << manager.TodayReview() << endl;

    // 输出所有复习项
    cout << manager.showEventByTime() << endl;

    // 删除复习项
    // manager.deleteEvent("some_id");

    // 释放资源
    manager.Free();

    return 0;
}