cmake_minimum_required(VERSION 3.10.0)
SET(PRONAME CYCLE)
project(${PRONAME} VERSION 0.1.0 LANGUAGES C CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set ( CMAKE_EXPORT_COMPILE_COMMANDS ON )

# 查找 GLFW
find_package(glfw3 REQUIRED)

# 查找OPENGL源文件
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)


file(GLOB SOURCEFILE
# ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/mainImgui.cpp
)

# imgui的源文件
file(GLOB IMGUISOURCEFILE
    ${CMAKE_CURRENT_SOURCE_DIR}/element/imgui/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui-1.91.9-docking/*.cpp
)
# glad的源文件
file(GLOB GLADSOURCEFILE
    ${CMAKE_CURRENT_SOURCE_DIR}/element/glad/src/*.c
)
# glfwLoader的源文件
file(GLOB GLFWSOURCEFILE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/glfwLoader/*.cpp
)


message("收集到的源文件： " 
${SOURCEFILE}
${IMGUISOURCEFILE}
)

add_executable(${PRONAME} 
    ${SOURCEFILE}
    ${IMGUISOURCEFILE}
)

# 链接 GLFW
target_link_libraries(${PRONAME} PRIVATE glfw OpenGL::GL)

target_include_directories(${PRONAME} PUBLIC
    # c++json的头文件目录
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/json
    # imgui的全局配置
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui-1.91.9-docking
    # imgui的opengl+glfw配置
    ${CMAKE_CURRENT_SOURCE_DIR}/element/imgui
    # glad的头文件
    ${CMAKE_CURRENT_SOURCE_DIR}/element/glad/include
    # glfwLoader的头文件
    ${CMAKE_CURRENT_SOURCE_DIR}/src/glfwLoader
)
