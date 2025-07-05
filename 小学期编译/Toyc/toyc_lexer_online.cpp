// ToyC语言词法分析器 - 在线编译器版本
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <cctype>

enum class TokenType {
    INT, VOID, IF, ELSE, WHILE, BREAK, CONTINUE, RETURN,
    IDENTIFIER, NUMBER,
    PLUS, MINUS, MULTIPLY, DIVIDE, MODULO,
    LESS, GREATER, LESS_EQUAL, GREATER_EQUAL, EQUAL, NOT_EQUAL,
    LOGICAL_AND, LOGICAL_OR, LOGICAL_NOT, ASSIGN,
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, SEMICOLON, COMMA,
    END_OF_FILE, INVALID
};

struct Token {
    TokenType type;
    std::string value;
    int line, column;
    Token(TokenType t, const std::string& v, int l, int c) : type(t), value(v), line(l), column(c) {}
};

class Lexer {
    std::string source;
    size_t current = 0;
    int line = 1, column = 1;
    std::unordered_set<std::string> keywords{"int", "void", "if", "else", "while", "break", "continue", "return"};

    char peek() const { return current >= source.length() ? '\0' : source[current]; }
    char peekNext() const { return current + 1 >= source.length() ? '\0' : source[current + 1]; }
    char advance() {
        if (current >= source.length()) return '\0';
        char c = source[current++];
        if (c == '\n') { line++; column = 1; } else { column++; }
        return c;
    }
    bool isAtEnd() const { return current >= source.length(); }

public:
    Lexer(const std::string& src) : source(src) {}

    Token nextToken() {
        // Skip whitespace
        while (!isAtEnd() && std::isspace(peek())) advance();
        
        if (isAtEnd()) return Token(TokenType::END_OF_FILE, "", line, column);
        
        int startLine = line, startColumn = column;
        char c = peek();
        
        // Comments
        if (c == '/' && peekNext() == '/') {
            advance(); advance();
            while (!isAtEnd() && peek() != '\n') advance();
            return nextToken();
        }
        if (c == '/' && peekNext() == '*') {
            advance(); advance();
            while (!isAtEnd() && !(peek() == '*' && peekNext() == '/')) advance();
            if (!isAtEnd()) { advance(); advance(); }
            return nextToken();
        }
        
        // Identifiers and keywords
        if (std::isalpha(c) || c == '_') {
            std::string value;
            while (!isAtEnd() && (std::isalnum(peek()) || peek() == '_')) value += advance();
            
            TokenType type = TokenType::IDENTIFIER;
            if (keywords.count(value)) {
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
        
        // Numbers
        if (std::isdigit(c) || (c == '-' && std::isdigit(peekNext()))) {
            std::string value;
            if (c == '-') value += advance();
            if (peek() == '0') value += advance();
            else while (!isAtEnd() && std::isdigit(peek())) value += advance();
            return Token(TokenType::NUMBER, value, startLine, startColumn);
        }
        
        // Operators and punctuation
        advance();
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
                if (peek() == '=') { advance(); return Token(TokenType::LESS_EQUAL, "<=", startLine, startColumn); }
                return Token(TokenType::LESS, "<", startLine, startColumn);
            case '>':
                if (peek() == '=') { advance(); return Token(TokenType::GREATER_EQUAL, ">=", startLine, startColumn); }
                return Token(TokenType::GREATER, ">", startLine, startColumn);
            case '=':
                if (peek() == '=') { advance(); return Token(TokenType::EQUAL, "==", startLine, startColumn); }
                return Token(TokenType::ASSIGN, "=", startLine, startColumn);
            case '!':
                if (peek() == '=') { advance(); return Token(TokenType::NOT_EQUAL, "!=", startLine, startColumn); }
                return Token(TokenType::LOGICAL_NOT, "!", startLine, startColumn);
            case '&':
                if (peek() == '&') { advance(); return Token(TokenType::LOGICAL_AND, "&&", startLine, startColumn); }
                break;
            case '|':
                if (peek() == '|') { advance(); return Token(TokenType::LOGICAL_OR, "||", startLine, startColumn); }
                break;
        }
        return Token(TokenType::INVALID, std::string(1, c), startLine, startColumn);
    }
    
    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        while (true) {
            Token token = nextToken();
            tokens.push_back(token);
            if (token.type == TokenType::END_OF_FILE || token.type == TokenType::INVALID) break;
        }
        return tokens;
    }
};

std::string tokenTypeToString(TokenType type) {
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

int main() {
    std::cout << "ToyC Lexer Demo\n================\n\n";
    
    // Test case 1: Basic program
    std::string code1 = R"(
        int main() {
            int x = 42;
            return x + 10;
        }
    )";
    
    std::cout << "Test 1 - Basic Program:\n";
    std::cout << code1 << "\n";
    std::cout << "Tokens:\n";
    
    Lexer lexer1(code1);
    auto tokens1 = lexer1.tokenize();
    for (const auto& token : tokens1) {
        std::cout << tokenTypeToString(token.type) << " '" << token.value << "' (" 
                  << token.line << ":" << token.column << ")\n";
    }
    
    // Test case 2: All operators
    std::string code2 = "+ - * / % < > <= >= == != && || ! =";
    std::cout << "\nTest 2 - Operators:\n" << code2 << "\n\nTokens:\n";
    
    Lexer lexer2(code2);
    auto tokens2 = lexer2.tokenize();
    for (const auto& token : tokens2) {
        std::cout << tokenTypeToString(token.type) << " '" << token.value << "'\n";
    }
    
    // Test case 3: With comments
    std::string code3 = R"(
        // This is a comment
        int factorial(int n) { /* recursive function */
            if (n <= 1) return 1;
            return n * factorial(n - 1);
        }
    )";
    
    std::cout << "\nTest 3 - With Comments:\n" << code3 << "\n";
    std::cout << "Tokens:\n";
    
    Lexer lexer3(code3);
    auto tokens3 = lexer3.tokenize();
    for (const auto& token : tokens3) {
        std::cout << tokenTypeToString(token.type) << " '" << token.value << "'\n";
    }
    
    return 0;
} 