#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <unordered_set>
#include <iostream>

// Token类型枚举
enum class TokenType {
    // 关键字
    INT,        // int
    VOID,       // void
    IF,         // if
    ELSE,       // else
    WHILE,      // while
    BREAK,      // break
    CONTINUE,   // continue
    RETURN,     // return
    
    // 标识符和字面量
    IDENTIFIER, // 标识符
    NUMBER,     // 整数
    
    // 运算符
    PLUS,       // +
    MINUS,      // -
    MULTIPLY,   // *
    DIVIDE,     // /
    MODULO,     // %
    
    // 比较运算符
    LESS,       // <
    GREATER,    // >
    LESS_EQUAL, // <=
    GREATER_EQUAL, // >=
    EQUAL,      // ==
    NOT_EQUAL,  // !=
    
    // 逻辑运算符
    LOGICAL_AND, // &&
    LOGICAL_OR,  // ||
    LOGICAL_NOT, // !
    
    // 赋值
    ASSIGN,     // =
    
    // 分隔符
    LEFT_PAREN,  // (
    RIGHT_PAREN, // )
    LEFT_BRACE,  // {
    RIGHT_BRACE, // }
    SEMICOLON,   // ;
    COMMA,       // ,
    
    // 特殊标记
    END_OF_FILE, // 文件结束
    INVALID      // 无效token
};

// Token结构体
struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;
    
    Token(TokenType t, const std::string& v, int l, int c) 
        : type(t), value(v), line(l), column(c) {}
};

// 词法分析器类
class Lexer {
private:
    std::string source;
    size_t current;
    int line;
    int column;
    std::unordered_set<std::string> keywords;
    
    // 私有方法
    char peek() const;
    char peekNext() const;
    char advance();
    bool isAtEnd() const;
    bool isAlpha(char c) const;
    bool isAlphaNumeric(char c) const;
    bool isDigit(char c) const;
    bool isWhitespace(char c) const;
    
    void skipWhitespace();
    void skipLineComment();
    void skipBlockComment();
    
    Token makeNumber();
    Token makeIdentifier();
    Token makeString(TokenType type, const std::string& value);
    
    void initKeywords();

public:
    Lexer(const std::string& source);
    Token nextToken();
    std::vector<Token> tokenize();
    
    // 工具方法
    static std::string tokenTypeToString(TokenType type);
    static void printTokens(const std::vector<Token>& tokens);
};

#endif // LEXER_H 