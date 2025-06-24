// main.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "codegen.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: toyc <source_file>\n";
        return 1;
    }

    // 读取源文件
    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "Error: Cannot open file " << argv[1] << "\n";
        return 1;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    // 词法分析
    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    // 调试打印词法单元
    std::cout << "Tokens:\n";
    for (const auto &tok : tokens) {
        std::cout << "  Type: " << static_cast<int>(tok.type) 
                  << ", Lexeme: '" << tok.lexeme 
                  << "', Line: " << tok.line << "\n";
    }

    // 语法分析
    Parser parser(tokens);
    std::vector<std::unique_ptr<FuncDef>> program;
    try {
        program = parser.parseCompUnit();
    } catch (const std::exception &ex) {
        std::cerr << "Parsing failed: " << ex.what() << "\n";
        return 1;
    }

    std::cout << "Parsing succeeded.\n";

    // 代码生成，传入 std::cout 作为输出流
    CodeGen codegen(std::cout);
    codegen.generate(program);

    return 0;
}