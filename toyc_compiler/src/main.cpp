// main.cpp
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "lexer.h"
#include "parser.h"
#include "semantic.h"
#include "codegen.h"

int main(int argc, char *argv[]) {
    std::string source;

    if (argc >= 2) {
        // 如果提供文件名，尝试从文件读取
        std::ifstream file(argv[1]);
        if (!file) {
            std::cerr << "Error: Cannot open file " << argv[1] << "\n";
            return 1;
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        source = buffer.str();
    } else {
        // 否则从 stdin 读取
        std::ostringstream buffer;
        buffer << std::cin.rdbuf();
        source = buffer.str();
    }

    try {
        // 词法分析
        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        // 可选调试：如果需要打印 Token 列表，写到 stderr
        std::cerr << "Tokens:\n";
        for (const auto &tok : tokens) {
            std::cerr << "  Type: " << static_cast<int>(tok.type)
                      << ", Lexeme: '" << tok.lexeme
                      << "', Line: " << tok.line << "\n";
        }

        // 语法分析
        Parser parser(tokens);
        auto program = parser.parseCompUnit();

        std::cerr << "Parsing succeeded.\n";

        // 语义分析
        SemanticAnalyzer semantic;
        semantic.analyze(program);
        std::cerr << "Semantic analysis succeeded.\n";

        // 汇编代码生成，唯一写到 stdout
        CodeGen codegen(std::cout);
        codegen.generate(program);

    } catch (const std::exception &ex) {
        std::cerr << "Compilation failed: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
