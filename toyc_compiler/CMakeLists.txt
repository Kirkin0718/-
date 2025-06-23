cmake_minimum_required(VERSION 3.10)
project(ToyCCompiler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 启用调试符号
set(CMAKE_BUILD_TYPE Debug)

# 项目源文件列表（按你的文件结构调整）
set(SOURCES
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
    src/semantic.cpp
    src/codegen.cpp
)

# 包含头文件目录
include_directories(include)

# 生成可执行文件
add_executable(toyc ${SOURCES})
