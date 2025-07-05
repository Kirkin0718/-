# ToyC 词法分析器 - 快速开始指南

## 🚀 30秒快速体验

### 选项1：在线编译器（无需安装）
1. 打开 [OnlineGDB](https://www.onlinegdb.com/) 
2. 选择 C++ 语言
3. 复制粘贴 `toyc_lexer_online.cpp` 的全部内容
4. 点击"Run"按钮
5. 查看词法分析结果！

### 选项2：本地编译（需要C++编译器）
```bash
# Windows (如果有编译器)
.\compile.bat

# Linux/macOS
make && ./toyc_lexer

# 或者直接编译
g++ -std=c++17 -o toyc_lexer main.cpp lexer.cpp
```

## 📝 测试你的ToyC代码

### 示例1：简单程序
```c
int main() {
    int x = 42;
    return x + 10;
}
```

### 示例2：函数调用
```c
int add(int a, int b) {
    return a + b;
}

int main() {
    int result = add(10, 20);
    return result;
}
```

### 示例3：控制流
```c
int main() {
    int i = 0;
    while (i < 10) {
        if (i == 5) {
            break;
        }
        i = i + 1;
    }
    return i;
}
```

## 🎯 支持的语言特性

### ✅ 已实现（词法分析）
- 所有ToyC关键字识别
- 标识符和数字常量
- 全部运算符和分隔符
- 单行和多行注释处理
- 精确的错误位置报告

### 🔄 下一步（语法分析）
- 语法解析器
- 抽象语法树(AST)构建
- 语义分析
- 代码生成

## 📊 输出格式

词法分析器输出每个token的详细信息：
```
Token: INT, Value: 'int', Line: 1, Column: 1
Token: IDENTIFIER, Value: 'main', Line: 1, Column: 5
Token: LEFT_PAREN, Value: '(', Line: 1, Column: 9
...
```

## 🛠️ 文件说明

| 文件 | 说明 | 用途 |
|------|------|------|
| `lexer.h/cpp` | 核心词法分析器 | 模块化实现 |
| `main.cpp` | 测试程序 | 本地编译测试 |
| `toyc_lexer_online.cpp` | 单文件版本 | 在线编译器使用 |
| `example.tc` | 示例ToyC代码 | 测试用例 |
| `compile.bat` | Windows编译脚本 | 自动检测编译器 |

## ❓ 常见问题

**Q: 没有C++编译器怎么办？**
A: 使用在线编译器或安装MinGW-w64

**Q: 如何测试自己的ToyC代码？**
A: 修改`toyc_lexer_online.cpp`中的测试用例

**Q: 支持哪些ToyC语法？**
A: 完全支持ToyC规范定义的所有词法单元

**Q: 发现bug怎么办？**
A: 检查代码是否符合ToyC语法规范

## 🎉 恭喜！

你已经成功运行了ToyC编译器的第一阶段！接下来可以：
1. 尝试分析更复杂的ToyC程序
2. 研究词法分析器的实现细节
3. 期待语法分析器的实现 