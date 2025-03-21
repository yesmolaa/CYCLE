cmake_minimum_required(VERSION 3.10.0)
SET(PRONAME CYCLE)
project(${PRONAME} VERSION 0.1.0 LANGUAGES C CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set ( CMAKE_EXPORT_COMPILE_COMMANDS ON )
add_compile_options(/utf-8)

# 查找 GLFW
#find_package(glfw3 REQUIRED)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw-master)


# 查找OPENGL源文件
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)


file(GLOB SOURCEFILE
# ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
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
# imguiLoader的源文件
# file(GLOB IMGUILOADERSOURCEFILE
#     ${CMAKE_CURRENT_SOURCE_DIR}/src/imguiLoader/*.cpp
# )
# jsonManager的源文件
file(GLOB JSONMANAGERSOURCEFILE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/jsonManagerLoader/*.cpp
)
# 可执行文件路径查找
file(GLOB PATHSORTSOURCEFILE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PathSystem/*.cpp
)





message("收集到的源文件： " 
${SOURCEFILE}
${IMGUISOURCEFILE}
${GLFWSOURCEFILE}
# ${IMGUILOADERSOURCEFILE}
)

add_executable(${PRONAME} 
    ${SOURCEFILE}
    ${IMGUISOURCEFILE}
    ${GLFWSOURCEFILE}
    # ${IMGUILOADERSOURCEFILE}
    ${JSONMANAGERSOURCEFILE}
    ${PATHSORTSOURCEFILE}
)

# 链接 GLFW
target_link_libraries(${PRONAME} PRIVATE glfw OpenGL::GL)





# 自己写的窗口的头文件
SET(MYWINDOWS
${CMAKE_CURRENT_SOURCE_DIR}/src/myWindows/creatItems
${CMAKE_CURRENT_SOURCE_DIR}/src/myWindows/setCycles
${CMAKE_CURRENT_SOURCE_DIR}/src/myWindows/showTargets
${CMAKE_CURRENT_SOURCE_DIR}/src/myWindows/todayTargets
${CMAKE_CURRENT_SOURCE_DIR}/src/myWindows/itemManager
)


target_include_directories(${PRONAME} PUBLIC
    # c++json的头文件目录
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/json/include
    # imgui的全局配置
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui-1.91.9-docking
    # imgui的opengl+glfw配置
    ${CMAKE_CURRENT_SOURCE_DIR}/element/imgui
    # glad的头文件
    ${CMAKE_CURRENT_SOURCE_DIR}/element/glad/include
    # glfwLoader的头文件
    ${CMAKE_CURRENT_SOURCE_DIR}/src/glfwLoader
    # imguiloader的头文件
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/imguiLoader
    # 自己的窗口头文件
    ${MYWINDOWS}
    # jsonmanager头文件
    ${CMAKE_CURRENT_SOURCE_DIR}/src/jsonManagerLoader
    # 可执行文件路径查找类
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PathSystem
)
