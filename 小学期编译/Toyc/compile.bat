@echo off
echo 正在编译ToyC词法分析器...

REM 尝试使用g++编译器
where g++ >nul 2>nul
if %ERRORLEVEL% == 0 (
    echo 使用g++编译器...
    g++ -std=c++17 -Wall -Wextra -O2 -g -o toyc_lexer.exe main.cpp lexer.cpp
    if %ERRORLEVEL% == 0 (
        echo 编译成功！
        echo 可执行文件：toyc_lexer.exe
        goto :end
    ) else (
        echo g++编译失败
    )
)

REM 尝试使用cl编译器（Visual Studio）
where cl >nul 2>nul
if %ERRORLEVEL% == 0 (
    echo 使用Microsoft Visual C++编译器...
    cl /EHsc /std:c++17 main.cpp lexer.cpp /Fe:toyc_lexer.exe
    if %ERRORLEVEL% == 0 (
        echo 编译成功！
        echo 可执行文件：toyc_lexer.exe
        del *.obj >nul 2>nul
        goto :end
    ) else (
        echo cl编译失败
    )
)

REM 尝试使用clang++编译器
where clang++ >nul 2>nul
if %ERRORLEVEL% == 0 (
    echo 使用clang++编译器...
    clang++ -std=c++17 -Wall -Wextra -O2 -g -o toyc_lexer.exe main.cpp lexer.cpp
    if %ERRORLEVEL% == 0 (
        echo 编译成功！
        echo 可执行文件：toyc_lexer.exe
        goto :end
    ) else (
        echo clang++编译失败
    )
)

echo 错误：未找到C++编译器！
echo 请安装以下任一编译器：
echo 1. MinGW-w64 (推荐) - https://www.mingw-w64.org/
echo 2. Microsoft Visual Studio (免费社区版)
echo 3. LLVM/Clang
echo.
echo 或者，您可以使用在线编译器测试代码。

:end
pause 