// main.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>

#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "semantic.h"
#include "codegen.h"

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        // 略：同上，保持不变
        // ...
        default: return "UNKNOWN";
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: toyc <source_file>\n";
        return 1;
    }

    // 读取源代码文件内容
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

    // 输出词法单元
    std::cout << "Tokens:\n";
    for (const auto &tok : tokens) {
        std::cout << "  Type: " << tokenTypeToString(tok.type)
                  << ", Lexeme: '" << tok.lexeme
                  << "', Line: " << tok.pos.line
                  << ", Column: " << tok.pos.column << "\n";
    }

    // 语法分析
    Parser parser(tokens);
    std::unique_ptr<ASTNode> ast = parser.parse();
    if (!ast) {
        std::cerr << "Parsing failed.\n";
        return 1;
    }
    std::cout << "Parsing succeeded.\n";

    // 语义分析
    SemanticAnalyzer semanticAnalyzer;
    try {
        semanticAnalyzer.analyze(ast.get());
        std::cout << "Semantic analysis passed.\n";
    } catch (const std::exception &e) {
        std::cerr << "Semantic analysis error: " << e.what() << "\n";
        return 1;
    }

    // 代码生成
    auto *program = dynamic_cast<Program*>(ast.get());
    if (!program) {
        std::cerr << "Error: AST root is not a Program node.\n";
        return 1;
    }

    CodeGen codegen(std::cout);
    codegen.generate(program->functions);

    return 0;
}