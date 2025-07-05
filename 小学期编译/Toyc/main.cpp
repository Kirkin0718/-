#include "lexer.h"
#include <iostream>
#include <fstream>
#include <sstream>

void testBasicTokens() {
    std::cout << "=== 测试基本Token ===\n";
    
    std::string code = R"(
        int main() {
            int x = 42;
            int y = -10;
            if (x > y) {
                return x + y;
            } else {
                return x - y;
            }
        }
    )";
    
    Lexer lexer(code);
    auto tokens = lexer.tokenize();
    Lexer::printTokens(tokens);
}

void testOperators() {
    std::cout << "\n=== 测试运算符 ===\n";
    
    std::string code = R"(
        + - * / % < > <= >= == != && || ! =
    )";
    
    Lexer lexer(code);
    auto tokens = lexer.tokenize();
    Lexer::printTokens(tokens);
}

void testComments() {
    std::cout << "\n=== 测试注释 ===\n";
    
    std::string code = R"(
        int main() { // 这是单行注释
            /* 这是
               多行注释 */
            int x = 10; /* 另一个多行注释 */
            return x;
        }
    )";
    
    Lexer lexer(code);
    auto tokens = lexer.tokenize();
    Lexer::printTokens(tokens);
}

void testComplexExpression() {
    std::cout << "\n=== 测试复杂表达式 ===\n";
    
    std::string code = R"(
        int factorial(int n) {
            if (n <= 1) {
                return 1;
            }
            return n * factorial(n - 1);
        }
        
        int main() {
            int result = factorial(5);
            while (result > 0) {
                result = result - 1;
                if (result == 10) {
                    break;
                }
                continue;
            }
            return result;
        }
    )";
    
    Lexer lexer(code);
    auto tokens = lexer.tokenize();
    Lexer::printTokens(tokens);
}

void testNumbers() {
    std::cout << "\n=== 测试数字 ===\n";
    
    std::string code = R"(
        0 123 -456 -0 999
    )";
    
    Lexer lexer(code);
    auto tokens = lexer.tokenize();
    Lexer::printTokens(tokens);
}

void testIdentifiers() {
    std::cout << "\n=== 测试标识符和关键字 ===\n";
    
    std::string code = R"(
        int void if else while break continue return
        _var var1 _123 myFunction test_var
    )";
    
    Lexer lexer(code);
    auto tokens = lexer.tokenize();
    Lexer::printTokens(tokens);
}

void testFromFile(const std::string& filename) {
    std::cout << "\n=== 测试文件: " << filename << " ===\n";
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "无法打开文件: " << filename << std::endl;
        return;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string code = buffer.str();
    
    Lexer lexer(code);
    auto tokens = lexer.tokenize();
    Lexer::printTokens(tokens);
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        // 如果提供了文件名参数，则分析该文件
        testFromFile(argv[1]);
    } else {
        // 否则运行内置测试
        testBasicTokens();
        testOperators();
        testComments();
        testNumbers();
        testIdentifiers();
        testComplexExpression();
    }
    
    return 0;
} 