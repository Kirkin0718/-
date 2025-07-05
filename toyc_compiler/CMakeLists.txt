cmake_minimum_required(VERSION 3.10)

# 项目名称
project(ToyCCompiler LANGUAGES CXX)

# 强制使用 C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 默认为 Debug 模式，可通过 -DCMAKE_BUILD_TYPE=Release 覆盖
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# 包含头文件目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# ===============================
# 编译主可执行文件: toyc
# ===============================
set(TOYC_SOURCES
  src/main.cpp
  src/lexer.cpp
  src/parser.cpp
  src/semantic.cpp
  src/codegen.cpp
)

add_executable(toyc ${TOYC_SOURCES})

# ===============================
# 编译单元测试: test_semantic
# ===============================
add_executable(test_semantic
  test/test_semantic.cpp
  src/semantic.cpp
  src/parser.cpp
  src/lexer.cpp
)

target_include_directories(test_semantic PRIVATE
  ${CMAKE_SOURCE_DIR}/include
)

# ===============================
# 打印编译信息
# ===============================
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Source Dir: ${PROJECT_SOURCE_DIR}")
message(STATUS "Binary Dir: ${PROJECT_BINARY_DIR}")
