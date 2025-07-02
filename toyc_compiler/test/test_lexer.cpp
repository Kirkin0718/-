#include "lexer.h"
#include <iostream>

void printToken(const Token& token) {
    std::cout << "[" << token.line << ":" << token.column << "] ";
    std::cout << "Type: ";

    switch (token.type) {
        case TokenType::INT: std::cout << "INT"; break;
        case TokenType::VOID: std::cout << "VOID"; break;
        case TokenType::IF: std::cout << "IF"; break;
        case TokenType::ELSE: std::cout << "ELSE"; break;
        case TokenType::WHILE: std::cout << "WHILE"; break;
        case TokenType::RETURN: std::cout << "RETURN"; break;
        case TokenType::BREAK: std::cout << "BREAK"; break;
        case TokenType::CONTINUE: std::cout << "CONTINUE"; break;
        case TokenType::IDENTIFIER: std::cout << "IDENTIFIER"; break;
        case TokenType::NUMBER: std::cout << "NUMBER"; break;
        case TokenType::PLUS: std::cout << "PLUS"; break;
        case TokenType::MINUS: std::cout << "MINUS"; break;
        case TokenType::MULTIPLY: std::cout << "MULTIPLY"; break;
        case TokenType::DIVIDE: std::cout << "DIVIDE"; break;
        case TokenType::MODULO: std::cout << "MODULO"; break;
        case TokenType::LESS: std::cout << "LESS"; break;
        case TokenType::GREATER: std::cout << "GREATER"; break;
        case TokenType::LESS_EQUAL: std::cout << "LESS_EQUAL"; break;
        case TokenType::GREATER_EQUAL: std::cout << "GREATER_EQUAL"; break;
        case TokenType::ASSIGN: std::cout << "ASSIGN"; break;
        case TokenType::EQUAL: std::cout << "EQUAL"; break;
        case TokenType::NOT_EQUAL: std::cout << "NOT_EQUAL"; break;
        case TokenType::LOGICAL_AND: std::cout << "LOGICAL_AND"; break;
        case TokenType::LOGICAL_OR: std::cout << "LOGICAL_OR"; break;
        case TokenType::NOT: std::cout << "NOT"; break;
        case TokenType::LPAREN: std::cout << "LPAREN"; break;
        case TokenType::RPAREN: std::cout << "RPAREN"; break;
        case TokenType::LBRACE: std::cout << "LBRACE"; break;
        case TokenType::RBRACE: std::cout << "RBRACE"; break;
        case TokenType::SEMICOLON: std::cout << "SEMICOLON"; break;
        case TokenType::COMMA: std::cout << "COMMA"; break;
        case TokenType::END_OF_FILE: std::cout << "EOF"; break;
        case TokenType::UNKNOWN: std::cout << "UNKNOWN"; break;
        default: std::cout << "UNEXPECTED"; break;
    }

    std::cout << ", Lexeme: \"" << token.lexeme << "\"\n";
}

int main() {
    std::string sampleCode = R"(
        int main() {
            int a = 42;
            if (a > 0) {
                a = a + 1;
            } else {
                a = a - 1;
            }
            return a;
        }
    )";

    try {
        Lexer lexer(sampleCode);
        auto tokens = lexer.tokenize();
        for (const auto& token : tokens) {
            printToken(token);
        }
    } catch (const std::exception& ex) {
        std::cerr << "Lexer error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
