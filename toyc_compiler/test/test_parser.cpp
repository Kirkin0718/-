// test_parser.cpp
#include "parser.h"
#include "token.h"
#include <iostream>
#include <vector>
#include <memory>

int main() {
    // 构造Token序列，模拟代码： int main() { return 42; }
    std::vector<Token> tokens = {
    {TokenType::INT, "int"},
    {TokenType::IDENTIFIER, "main"},
    {TokenType::LPAREN, "("},
    {TokenType::RPAREN, ")"},
    {TokenType::LBRACE, "{"},
    {TokenType::RETURN, "return"},
    {TokenType::NUMBER, "42"},
    {TokenType::SEMICOLON, ";"},
    {TokenType::RBRACE, "}"},
    {TokenType::END_OF_FILE, ""}
};

    Parser parser(std::move(tokens));

    try {
        auto funcs = parser.parseCompUnit();
        std::cout << "Parsing succeeded. Parsed " << funcs.size() << " functions.\n";
    } catch (const std::exception& e) {
        std::cerr << "Parsing failed: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
