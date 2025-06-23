#include "lexer.h"
#include <cctype>
#include <unordered_map>

Lexer::Lexer(const std::string &src) : source(src), pos(0), line(1), column(1) {}

char Lexer::peek() const {
    if (pos >= source.size()) return '\0';
    return source[pos];
}

char Lexer::peekNext() const {
    if (pos + 1 >= source.size()) return '\0';
    return source[pos + 1];
}

char Lexer::advance() {
    char ch = source[pos++];
    if (ch == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    return ch;
}

void Lexer::skipWhitespaceAndComments() {
    while (pos < source.size()) {
        char ch = peek();
        if (isspace(ch)) {
            advance();
        } else if (ch == '/' && peekNext() == '/') {
            // 单行注释
            advance(); advance();
            while (pos < source.size() && peek() != '\n') {
                advance();
            }
        } else if (ch == '/' && peekNext() == '*') {
            // 多行注释
            advance(); advance();
            while (pos < source.size()) {
                if (peek() == '*' && peekNext() == '/') {
                    advance(); advance();
                    break;
                }
                advance();
            }
        } else {
            break;
        }
    }
}

Token Lexer::makeToken(TokenType type, const std::string &lexeme, Position pos) {
    return Token(type, lexeme, pos);
}

Token Lexer::scanIdentifierOrKeyword() {
    Position startPos(line, column);
    size_t start = pos;
    while (pos < source.size() && (isalnum(peek()) || peek() == '_')) {
        advance();
    }
    std::string text = source.substr(start, pos - start);

    static const std::unordered_map<std::string, TokenType> keywords = {
        {"int", TokenType::INT},
        {"void", TokenType::VOID},
        {"if", TokenType::IF},
        {"else", TokenType::ELSE},
        {"while", TokenType::WHILE},
        {"break", TokenType::BREAK},
        {"continue", TokenType::CONTINUE},
        {"return", TokenType::RETURN},
    };

    auto it = keywords.find(text);
    if (it != keywords.end()) {
        return makeToken(it->second, text, startPos);
    }
    return makeToken(TokenType::IDENTIFIER, text, startPos);
}

Token Lexer::scanNumber() {
    Position startPos(line, column);
    size_t start = pos;
    while (pos < source.size() && isdigit(peek())) {
        advance();
    }
    std::string text = source.substr(start, pos - start);
    return makeToken(TokenType::NUMBER, text, startPos);
}

Token Lexer::scanOperatorOrDelimiter() {
    Position startPos(line, column);
    char ch = advance();

    switch (ch) {
        case '+': return makeToken(TokenType::PLUS, "+", startPos);
        case '-': return makeToken(TokenType::MINUS, "-", startPos);
        case '*': return makeToken(TokenType::MULTIPLY, "*", startPos);
        case '/': return makeToken(TokenType::DIVIDE, "/", startPos);
        case '%': return makeToken(TokenType::MODULO, "%", startPos);
        case ';': return makeToken(TokenType::SEMICOLON, ";", startPos);
        case ',': return makeToken(TokenType::COMMA, ",", startPos);
        case '(': return makeToken(TokenType::LPAREN, "(", startPos);
        case ')': return makeToken(TokenType::RPAREN, ")", startPos);
        case '{': return makeToken(TokenType::LBRACE, "{", startPos);
        case '}': return makeToken(TokenType::RBRACE, "}", startPos);
        case '=':
            if (peek() == '=') {
                advance();
                return makeToken(TokenType::EQUAL, "==", startPos);
            } else {
                return makeToken(TokenType::ASSIGN, "=", startPos);
            }
        case '!':
            if (peek() == '=') {
                advance();
                return makeToken(TokenType::NOT_EQUAL, "!=", startPos);
            } else {
                return makeToken(TokenType::UNKNOWN, "!", startPos);
            }
        case '<':
            if (peek() == '=') {
                advance();
                return makeToken(TokenType::LESS_EQUAL, "<=", startPos);
            } else {
                return makeToken(TokenType::LESS, "<", startPos);
            }
        case '>':
            if (peek() == '=') {
                advance();
                return makeToken(TokenType::GREATER_EQUAL, ">=", startPos);
            } else {
                return makeToken(TokenType::GREATER, ">", startPos);
            }
        case '&':
            if (peek() == '&') {
                advance();
                return makeToken(TokenType::LOGICAL_AND, "&&", startPos);
            }
            break;
        case '|':
            if (peek() == '|') {
                advance();
                return makeToken(TokenType::LOGICAL_OR, "||", startPos);
            }
            break;
    }

    return makeToken(TokenType::UNKNOWN, std::string(1, ch), startPos);
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (true) {
        skipWhitespaceAndComments();
        if (pos >= source.size()) {
            tokens.push_back(makeToken(TokenType::END_OF_FILE, "", Position(line, column)));
            break;
        }

        char ch = peek();
        if (isalpha(ch) || ch == '_') {
            tokens.push_back(scanIdentifierOrKeyword());
        } else if (isdigit(ch)) {
            tokens.push_back(scanNumber());
        } else {
            tokens.push_back(scanOperatorOrDelimiter());
        }
    }
    return tokens;
}