#include "lexer.h"
#include <cctype>
#include <stdexcept>

Lexer::Lexer(const std::string& source) 
    : source(source), current(0), line(1), column(1) {
    initKeywords();
}

void Lexer::initKeywords() {
    keywords = {
        "int", "void", "if", "else", "while", 
        "break", "continue", "return"
    };
}

char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return source[current];
}

char Lexer::peekNext() const {
    if (current + 1 >= source.length()) return '\0';
    return source[current + 1];
}

char Lexer::advance() {
    if (isAtEnd()) return '\0';
    
    char c = source[current++];
    if (c == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    return c;
}

bool Lexer::isAtEnd() const {
    return current >= source.length();
}

bool Lexer::isAlpha(char c) const {
    return std::isalpha(c) || c == '_';
}

bool Lexer::isAlphaNumeric(char c) const {
    return std::isalnum(c) || c == '_';
}

bool Lexer::isDigit(char c) const {
    return std::isdigit(c);
}

bool Lexer::isWhitespace(char c) const {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

void Lexer::skipWhitespace() {
    while (!isAtEnd() && isWhitespace(peek())) {
        advance();
    }
}

void Lexer::skipLineComment() {
    // 跳过 "//" 开始的单行注释
    while (!isAtEnd() && peek() != '\n') {
        advance();
    }
}

void Lexer::skipBlockComment() {
    // 跳过 "/*" 开始的多行注释
    while (!isAtEnd()) {
        if (peek() == '*' && peekNext() == '/') {
            advance(); // 跳过 '*'
            advance(); // 跳过 '/'
            break;
        }
        advance();
    }
}

Token Lexer::makeNumber() {
    int startLine = line;
    int startColumn = column;
    std::string value;
    
    // 处理可能的负号
    if (peek() == '-') {
        value += advance();
    }
    
    // 处理数字部分
    if (peek() == '0') {
        // 如果是0，只能是单独的0
        value += advance();
    } else {
        // 非零开头的数字
        while (!isAtEnd() && isDigit(peek())) {
            value += advance();
        }
    }
    
    return Token(TokenType::NUMBER, value, startLine, startColumn);
}

Token Lexer::makeIdentifier() {
    int startLine = line;
    int startColumn = column;
    std::string value;
    
    // 第一个字符必须是字母或下划线
    value += advance();
    
    // 后续字符可以是字母、数字或下划线
    while (!isAtEnd() && isAlphaNumeric(peek())) {
        value += advance();
    }
    
    // 检查是否为关键字
    TokenType type = TokenType::IDENTIFIER;
    if (keywords.find(value) != keywords.end()) {
        if (value == "int") type = TokenType::INT;
        else if (value == "void") type = TokenType::VOID;
        else if (value == "if") type = TokenType::IF;
        else if (value == "else") type = TokenType::ELSE;
        else if (value == "while") type = TokenType::WHILE;
        else if (value == "break") type = TokenType::BREAK;
        else if (value == "continue") type = TokenType::CONTINUE;
        else if (value == "return") type = TokenType::RETURN;
    }
    
    return Token(type, value, startLine, startColumn);
}

Token Lexer::makeString(TokenType type, const std::string& value) {
    return Token(type, value, line, column);
}

Token Lexer::nextToken() {
    skipWhitespace();
    
    if (isAtEnd()) {
        return Token(TokenType::END_OF_FILE, "", line, column);
    }
    
    int startLine = line;
    int startColumn = column;
    char c = peek();
    
    // 处理注释
    if (c == '/' && peekNext() == '/') {
        advance(); // 跳过第一个 '/'
        advance(); // 跳过第二个 '/'
        skipLineComment();
        return nextToken(); // 递归调用获取下一个token
    }
    
    if (c == '/' && peekNext() == '*') {
        advance(); // 跳过 '/'
        advance(); // 跳过 '*'
        skipBlockComment();
        return nextToken(); // 递归调用获取下一个token
    }
    
    // 处理标识符和关键字
    if (isAlpha(c)) {
        return makeIdentifier();
    }
    
    // 处理数字（包括负数）
    if (isDigit(c) || (c == '-' && isDigit(peekNext()))) {
        return makeNumber();
    }
    
    // 处理单字符和多字符运算符
    advance(); // 消费当前字符
    
    switch (c) {
        case '+': return Token(TokenType::PLUS, "+", startLine, startColumn);
        case '-': return Token(TokenType::MINUS, "-", startLine, startColumn);
        case '*': return Token(TokenType::MULTIPLY, "*", startLine, startColumn);
        case '/': return Token(TokenType::DIVIDE, "/", startLine, startColumn);
        case '%': return Token(TokenType::MODULO, "%", startLine, startColumn);
        case '(': return Token(TokenType::LEFT_PAREN, "(", startLine, startColumn);
        case ')': return Token(TokenType::RIGHT_PAREN, ")", startLine, startColumn);
        case '{': return Token(TokenType::LEFT_BRACE, "{", startLine, startColumn);
        case '}': return Token(TokenType::RIGHT_BRACE, "}", startLine, startColumn);
        case ';': return Token(TokenType::SEMICOLON, ";", startLine, startColumn);
        case ',': return Token(TokenType::COMMA, ",", startLine, startColumn);
        
        case '<':
            if (peek() == '=') {
                advance();
                return Token(TokenType::LESS_EQUAL, "<=", startLine, startColumn);
            }
            return Token(TokenType::LESS, "<", startLine, startColumn);
            
        case '>':
            if (peek() == '=') {
                advance();
                return Token(TokenType::GREATER_EQUAL, ">=", startLine, startColumn);
            }
            return Token(TokenType::GREATER, ">", startLine, startColumn);
            
        case '=':
            if (peek() == '=') {
                advance();
                return Token(TokenType::EQUAL, "==", startLine, startColumn);
            }
            return Token(TokenType::ASSIGN, "=", startLine, startColumn);
            
        case '!':
            if (peek() == '=') {
                advance();
                return Token(TokenType::NOT_EQUAL, "!=", startLine, startColumn);
            }
            return Token(TokenType::LOGICAL_NOT, "!", startLine, startColumn);
            
        case '&':
            if (peek() == '&') {
                advance();
                return Token(TokenType::LOGICAL_AND, "&&", startLine, startColumn);
            }
            break; // 单个&不是有效的token
            
        case '|':
            if (peek() == '|') {
                advance();
                return Token(TokenType::LOGICAL_OR, "||", startLine, startColumn);
            }
            break; // 单个|不是有效的token
            
        default:
            break;
    }
    
    // 如果到这里，说明遇到了无效字符
    std::string invalid_char(1, c);
    return Token(TokenType::INVALID, invalid_char, startLine, startColumn);
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    
    while (true) {
        Token token = nextToken();
        tokens.push_back(token);
        
        if (token.type == TokenType::END_OF_FILE || token.type == TokenType::INVALID) {
            break;
        }
    }
    
    return tokens;
}

std::string Lexer::tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::INT: return "INT";
        case TokenType::VOID: return "VOID";
        case TokenType::IF: return "IF";
        case TokenType::ELSE: return "ELSE";
        case TokenType::WHILE: return "WHILE";
        case TokenType::BREAK: return "BREAK";
        case TokenType::CONTINUE: return "CONTINUE";
        case TokenType::RETURN: return "RETURN";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::MULTIPLY: return "MULTIPLY";
        case TokenType::DIVIDE: return "DIVIDE";
        case TokenType::MODULO: return "MODULO";
        case TokenType::LESS: return "LESS";
        case TokenType::GREATER: return "GREATER";
        case TokenType::LESS_EQUAL: return "LESS_EQUAL";
        case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
        case TokenType::EQUAL: return "EQUAL";
        case TokenType::NOT_EQUAL: return "NOT_EQUAL";
        case TokenType::LOGICAL_AND: return "LOGICAL_AND";
        case TokenType::LOGICAL_OR: return "LOGICAL_OR";
        case TokenType::LOGICAL_NOT: return "LOGICAL_NOT";
        case TokenType::ASSIGN: return "ASSIGN";
        case TokenType::LEFT_PAREN: return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
        case TokenType::LEFT_BRACE: return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::COMMA: return "COMMA";
        case TokenType::END_OF_FILE: return "END_OF_FILE";
        case TokenType::INVALID: return "INVALID";
        default: return "UNKNOWN";
    }
}

void Lexer::printTokens(const std::vector<Token>& tokens) {
    for (const auto& token : tokens) {
        std::cout << "Token: " << tokenTypeToString(token.type) 
                  << ", Value: '" << token.value 
                  << "', Line: " << token.line 
                  << ", Column: " << token.column << std::endl;
    }
} 