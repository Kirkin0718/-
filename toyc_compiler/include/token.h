#ifndef TOKEN_H
#define TOKEN_H
#include <string>

enum class TokenType {
    INT, VOID, IF, ELSE, WHILE, RETURN, BREAK, CONTINUE,
    IDENTIFIER, NUMBER,
    PLUS, MINUS, MULTIPLY, DIVIDE, MODULO,
    LESS, GREATER, LESS_EQUAL, GREATER_EQUAL,
    ASSIGN, EQUAL, NOT_EQUAL, LOGICAL_AND, LOGICAL_OR, NOT,
    LPAREN, RPAREN, LBRACE, RBRACE, COMMA, SEMICOLON,
    END_OF_FILE, UNKNOWN
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
    int column;

    Token() = default;

    Token(TokenType t, const std::string &lex, int ln, int col)
        : type(t), lexeme(lex), line(ln), column(col) {}
};

#endif