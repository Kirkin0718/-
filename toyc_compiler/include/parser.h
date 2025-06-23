#pragma once
#include "lexer.h"
#include "ast.h"
#include <vector>
#include <memory>
#include <stdexcept>

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {}

    // 入口函数，解析完整程序
    std::unique_ptr<ASTNode> parse();

private:
    // 表达式解析，返回 Expr 类型指针
    std::unique_ptr<Expr> parseExpr();
    std::unique_ptr<Expr> parseLOrExpr();
    std::unique_ptr<Expr> parseLAndExpr();
    std::unique_ptr<Expr> parseRelExpr();
    std::unique_ptr<Expr> parseAddExpr();
    std::unique_ptr<Expr> parseMulExpr();
    std::unique_ptr<Expr> parseUnaryExpr();
    std::unique_ptr<Expr> parsePrimaryExpr();

    // 语句解析，返回 Stmt 类型指针
    std::unique_ptr<Stmt> parseStmt();
    std::unique_ptr<Stmt> parseBlock();
    std::unique_ptr<Stmt> parseIfStmt();
    std::unique_ptr<Stmt> parseWhileStmt();
    std::unique_ptr<Stmt> parseReturnStmt();
    std::unique_ptr<Stmt> parseDeclOrAssignOrExpr();

    // 函数定义解析
    std::unique_ptr<ASTNode> parseFuncDef();
    std::vector<std::unique_ptr<ASTNode>> parseParamList();

    // 工具函数
    const Token& peek() const;
    const Token& advance();
    bool match(TokenType type);
    bool check(TokenType type) const;
    bool isAtEnd() const;
    const Token& expect(TokenType type, const std::string& msg);

    const std::vector<Token>& tokens;
    size_t current = 0;
};
