# ToyC 编译器 - 词法分析器

这是ToyC语言编译器的第一阶段实现：词法分析器。

## 项目概述

ToyC是C语言的一个简化子集，本项目旨在实现一个完整的编译器，将ToyC源文件编译成RISC-V32汇编代码。

### 当前实现：词法分析器

词法分析器能够识别ToyC语言的所有词法单元（Token），包括：

#### 关键字
- `int`, `void`, `if`, `else`, `while`, `break`, `continue`, `return`

#### 标识符
- 正则表达式：`[_A-Za-z][_A-Za-z0-9]*`
- 例如：`main`, `_var`, `variable1`, `myFunction`

#### 整数字面量
- 正则表达式：`-?(0|[1-9][0-9]*)`
- 例如：`0`, `123`, `-456`, `999`

#### 运算符
- 算术运算符：`+`, `-`, `*`, `/`, `%`
- 比较运算符：`<`, `>`, `<=`, `>=`, `==`, `!=`
- 逻辑运算符：`&&`, `||`, `!`
- 赋值运算符：`=`

#### 分隔符
- 括号：`(`, `)`
- 大括号：`{`, `}`
- 标点：`;`, `,`

#### 注释支持
- 单行注释：`// 注释内容`
- 多行注释：`/* 注释内容 */`

## 编译和运行

### Windows环境编译

#### 方法1：使用批处理文件（推荐）
```cmd
.\compile.bat
```

#### 方法2：手动编译
如果已安装C++编译器，可以使用以下命令之一：

**使用g++（MinGW）：**
```cmd
g++ -std=c++17 -Wall -Wextra -O2 -g -o toyc_lexer.exe main.cpp lexer.cpp
```

**使用Microsoft Visual C++：**
```cmd
cl /EHsc /std:c++17 main.cpp lexer.cpp /Fe:toyc_lexer.exe
```

**使用clang++：**
```cmd
clang++ -std=c++17 -Wall -Wextra -O2 -g -o toyc_lexer.exe main.cpp lexer.cpp
```

### Linux/macOS环境编译
```bash
make
```

### 如果没有C++编译器

#### 安装编译器选项：
1. **MinGW-w64（推荐）**: https://www.mingw-w64.org/
2. **Microsoft Visual Studio Community（免费）**: https://visualstudio.microsoft.com/
3. **LLVM/Clang**: https://llvm.org/

#### 在线编译器测试：
- **Compiler Explorer**: https://godbolt.org/
- **OnlineGDB**: https://www.onlinegdb.com/
- **Replit**: https://replit.com/

**使用在线编译器的步骤：**
1. 复制 `toyc_lexer_online.cpp` 文件的内容
2. 打开任一在线编译器
3. 粘贴代码并选择C++编译器
4. 运行程序查看词法分析结果

**推荐的在线编译器设置：**
- 编译器：GCC 9.0+ 或 Clang 10.0+
- 标准：C++17
- 优化级别：-O2

### 运行测试
```cmd
# Windows
.\toyc_lexer.exe

# Linux/macOS  
./toyc_lexer
```

### 分析特定文件
```cmd
# Windows
.\toyc_lexer.exe filename.tc

# Linux/macOS
./toyc_lexer filename.tc
```

## 项目结构

```
.
├── lexer.h                 # 词法分析器头文件
├── lexer.cpp               # 词法分析器实现
├── main.cpp                # 测试程序主文件
├── toyc_lexer_online.cpp   # 在线编译器单文件版本
├── example.tc              # 示例ToyC源文件
├── compile.bat             # Windows编译批处理脚本
├── Makefile                # Linux/macOS编译配置
└── README.md               # 项目说明文档
```

## 设计特点

### DFA实现
词法分析器基于确定有限自动机(DFA)设计，具有以下特点：

1. **高效的字符识别**：使用状态机方式识别不同类型的Token
2. **错误处理**：能够识别并报告非法字符
3. **位置跟踪**：记录每个Token的行号和列号，便于错误报告
4. **注释忽略**：正确处理单行和多行注释

### 核心算法

1. **数字识别**：严格按照ToyC规范识别整数（包括负数和零）
2. **标识符识别**：区分关键字和用户定义标识符
3. **运算符识别**：支持单字符和多字符运算符
4. **空白字符处理**：忽略所有空白字符（空格、制表符、换行符）

## 示例输出

对于输入代码：
```c
int main() {
    int x = 42;
    return x + 10;
}
```

词法分析器输出：
```
Token: INT, Value: 'int', Line: 1, Column: 1
Token: IDENTIFIER, Value: 'main', Line: 1, Column: 5
Token: LEFT_PAREN, Value: '(', Line: 1, Column: 9
Token: RIGHT_PAREN, Value: ')', Line: 1, Column: 10
Token: LEFT_BRACE, Value: '{', Line: 1, Column: 12
Token: INT, Value: 'int', Line: 2, Column: 5
Token: IDENTIFIER, Value: 'x', Line: 2, Column: 9
Token: ASSIGN, Value: '=', Line: 2, Column: 11
Token: NUMBER, Value: '42', Line: 2, Column: 13
Token: SEMICOLON, Value: ';', Line: 2, Column: 15
Token: RETURN, Value: 'return', Line: 3, Column: 5
Token: IDENTIFIER, Value: 'x', Line: 3, Column: 12
Token: PLUS, Value: '+', Line: 3, Column: 14
Token: NUMBER, Value: '10', Line: 3, Column: 16
Token: SEMICOLON, Value: ';', Line: 3, Column: 18
Token: RIGHT_BRACE, Value: '}', Line: 4, Column: 1
Token: END_OF_FILE, Value: '', Line: 4, Column: 2
```

## 技术规范

- **编程语言**：C++17
- **编译器**：GCC或兼容编译器
- **依赖**：标准C++库
- **目标平台**：跨平台

## 下一步计划

- [ ] 语法分析器（Parser）
- [ ] 抽象语法树（AST）构建
- [ ] 语义分析
- [ ] 中间代码生成
- [ ] RISC-V32代码生成
- [ ] 优化算法实现

## 测试覆盖

词法分析器经过全面测试，涵盖：
- 所有关键字识别
- 各种标识符格式
- 正负整数识别
- 所有运算符组合
- 注释处理
- 错误字符检测
- 边界情况处理 