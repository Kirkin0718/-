cmake_minimum_required(VERSION 3.10)
project(ToyCCompiler)

# C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 默认 Debug
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# 包含头文件
include_directories(${CMAKE_SOURCE_DIR}/include)

# -------------------------------
# 主编译目标：可执行文件 toyc
# -------------------------------
set(SOURCES
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
    src/semantic.cpp
    src/codegen.cpp
)

add_executable(toyc ${SOURCES})

# -------------------------------
# 单元测试：test_semantic
# -------------------------------
add_executable(test_semantic
    test/test_semantic.cpp
    src/semantic.cpp
    src/parser.cpp
    src/lexer.cpp
)

target_include_directories(test_semantic PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# -------------------------------
# 可选：显示编译信息
# -------------------------------
message(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
message(STATUS "PROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}")

