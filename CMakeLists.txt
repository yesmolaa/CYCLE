cmake_minimum_required(VERSION 3.10.0)
SET(PRONAME CYCLE)
project(${PRONAME} VERSION 0.1.0 LANGUAGES C CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set ( CMAKE_EXPORT_COMPILE_COMMANDS ON )

add_executable(${PRONAME} main.cpp)

target_include_directories(${PRONAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor
    
)
