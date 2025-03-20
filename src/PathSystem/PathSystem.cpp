#include "PathSystem.h"


PathSystem::PathSystem()
{
    
}

std::string PathSystem::getExecutableDir()
{
    std::string exePath;

#ifdef _WIN32
    char path[MAX_PATH];
    if (GetModuleFileNameA(NULL, path, MAX_PATH) == 0) {
        std::cerr << "GetModuleFileNameA failed" << std::endl;
        return "";
    }
    exePath = path;
#elif __APPLE__
    char path[PATH_MAX];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) != 0) {
        std::cerr << "_NSGetExecutablePath failed" << std::endl;
        return "";
    }
    exePath = path;
#else // Linux and other Unix-like systems
    char path[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", path, sizeof(path) - 1);
    if (len == -1) {
        std::cerr << "readlink failed" << std::endl;
        return "";
    }
    path[len] = '\0';
    exePath = path;
#endif

    // 提取目录部分
    size_t pos = exePath.find_last_of("/\\");
    if (pos != std::string::npos) {
        exePath = exePath.substr(0, pos + 1); // 包含斜杠或反斜杠
    } else {
        // 如果无法找到分隔符，则当前目录作为默认
        char cwd[PATH_MAX];
        if (GetCurrentDir(cwd, sizeof(cwd)) != nullptr) {
            exePath = cwd;
            exePath += "/";
        } else {
            std::cerr << "GetCurrentDir failed" << std::endl;
            exePath = "";
        }
    }

    return exePath;
}