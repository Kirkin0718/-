#include "lexer.h"
#include <iostream>

void printToken(const Token& token) {
    std::cout << "[" << token.pos.line << ":" << token.pos.column << "] ";
    std::cout << "Type: ";

    switch (token.type) {
        case TokenType::KW_INT: std::cout << "KW_INT"; break;
        case TokenType::KW_VOID: std::cout << "KW_VOID"; break;
        case TokenType::KW_IF: std::cout << "KW_IF"; break;
        case TokenType::KW_ELSE: std::cout << "KW_ELSE"; break;
        case TokenType::KW_WHILE: std::cout << "KW_WHILE"; break;
        case TokenType::KW_RETURN: std::cout << "KW_RETURN"; break;
        case TokenType::KW_BREAK: std::cout << "KW_BREAK"; break;
        case TokenType::KW_CONTINUE: std::cout << "KW_CONTINUE"; break;
        case TokenType::IDENTIFIER: std::cout << "IDENTIFIER"; break;
        case TokenType::NUMBER: std::cout << "NUMBER"; break;
        case TokenType::PLUS: std::cout << "PLUS"; break;
        case TokenType::MINUS: std::cout << "MINUS"; break;
        case TokenType::MUL: std::cout << "MUL"; break;
        case TokenType::DIV: std::cout << "DIV"; break;
        case TokenType::MOD: std::cout << "MOD"; break;
        case TokenType::LT: std::cout << "LT"; break;
        case TokenType::LE: std::cout << "LE"; break;
        case TokenType::GT: std::cout << "GT"; break;
        case TokenType::GE: std::cout << "GE"; break;
        case TokenType::EQ: std::cout << "EQ"; break;
        case TokenType::NE: std::cout << "NE"; break;
        case TokenType::ASSIGN: std::cout << "ASSIGN"; break;
        case TokenType::AND: std::cout << "AND"; break;
        case TokenType::OR: std::cout << "OR"; break;
        case TokenType::NOT: std::cout << "NOT"; break;
        case TokenType::LPAREN: std::cout << "LPAREN"; break;
        case TokenType::RPAREN: std::cout << "RPAREN"; break;
        case TokenType::LBRACE: std::cout << "LBRACE"; break;
        case TokenType::RBRACE: std::cout << "RBRACE"; break;
        case TokenType::SEMICOLON: std::cout << "SEMICOLON"; break;
        case TokenType::COMMA: std::cout << "COMMA"; break;
        case TokenType::END_OF_FILE: std::cout << "EOF"; break;
        default: std::cout << "UNKNOWN"; break;
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
