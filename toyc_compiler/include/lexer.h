#ifndef TOYC_LEXER_H
#define TOYC_LEXER_H

#include <string>
#include <vector>
#include <cstdint>

enum class TokenType {
    // Keywords
    KW_INT, KW_VOID, KW_IF, KW_ELSE, KW_WHILE, KW_RETURN,
    KW_BREAK, KW_CONTINUE,

    // Operators & Punctuators
    PLUS, MINUS, MUL, DIV, MOD,
    LT, GT, LE, GE, EQ, NE,
    ASSIGN, AND, OR, NOT,
    LPAREN, RPAREN, LBRACE, RBRACE, SEMICOLON, COMMA,

    // Literals & ID
    IDENTIFIER, NUMBER,

    // Special
    END_OF_FILE,
    INVALID
};

struct Position {
    int line;
    int column;
};

struct Token {
    TokenType type;
    std::string lexeme;
    Position pos;

    Token(TokenType t, std::string lex, Position p)
        : type(t), lexeme(std::move(lex)), pos(p) {}
};

class Lexer {
public:
    explicit Lexer(const std::string& source);
    std::vector<Token> tokenize();

private:
    char peek() const;
    char advance();
    bool match(char expected);
    void skipWhitespaceAndComments();
    Token identifier();
    Token number();
    Token makeToken(TokenType type, const std::string& lexeme, Position pos);

    const std::string& src;
    size_t index = 0;
    int line = 1;
    int column = 1;
};

#endif // TOYC_LEXER_H
