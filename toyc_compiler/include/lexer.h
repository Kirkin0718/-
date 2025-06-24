#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include "token.h"

class Lexer {
private:
    const std::string &source;  // 代码源文本
    size_t index;
    int line;
    int column;

    char peek() const;
    char peekNext() const;
    char advance();

    void skipWhitespaceAndComments();

    Token makeToken(TokenType type, const std::string &lexeme);

    Token identifier();
    Token number();
    Token operatorOrDelimiter();

public:
    explicit Lexer(const std::string &src);

    std::vector<Token> tokenize();
};

#endif // LEXER_H
