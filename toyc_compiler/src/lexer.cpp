#include "lexer.h"
#include "token.h"
#include <cctype>
#include <vector>
#include <string>

Lexer::Lexer(const std::string &src)
    : source(src), index(0), line(1), column(1) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (true) {
        skipWhitespaceAndComments();
        if (index >= source.size()) {
            tokens.emplace_back(TokenType::EndOfFile, "", line, column);
            break;
        }

        char c = peek();
        if (std::isalpha(c) || c == '_') {
            tokens.push_back(identifier());
        } else if (std::isdigit(c)) {
            tokens.push_back(number());
        } else {
            tokens.push_back(operatorOrDelimiter());
        }
    }

    return tokens;
}

char Lexer::peek() const {
    if (index >= source.size()) return '\0';
    return source[index];
}

char Lexer::peekNext() const {
    if (index + 1 >= source.size()) return '\0';
    return source[index + 1];
}

char Lexer::advance() {
    char c = source[index++];
    if (c == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    return c;
}

void Lexer::skipWhitespaceAndComments() {
    while (index < source.size()) {
        char c = peek();
        if (isspace(c)) {
            advance();
            continue;
        }
        if (c == '/' && peekNext() == '/') {
            // 跳过单行注释
            while (peek() != '\n' && index < source.size()) advance();
            continue;
        }
        break;
    }
}

Token Lexer::makeToken(TokenType type, const std::string &lexeme) {
    // 生成Token，位置回退到词素开始列
    int tokenColumn = column - static_cast<int>(lexeme.size());
    if (tokenColumn < 1) tokenColumn = 1; // 防止越界
    return Token(type, lexeme, line, tokenColumn);
}

Token Lexer::identifier() {
    size_t start = index;
    while (isalnum(peek()) || peek() == '_') {
        advance();
    }
    std::string lexeme = source.substr(start, index - start);
    return makeToken(TokenType::Identifier, lexeme);
}

Token Lexer::number() {
    size_t start = index;
    while (isdigit(peek())) {
        advance();
    }
    std::string lexeme = source.substr(start, index - start);
    return makeToken(TokenType::Number, lexeme);
}

Token Lexer::operatorOrDelimiter() {
    char c = advance();
    std::string lexeme(1, c);
    // TODO: 可扩展多字符操作符判定
    return makeToken(TokenType::Operator, lexeme);
}
