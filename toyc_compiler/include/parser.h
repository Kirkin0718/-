#pragma once
#include "token.h"
#include "ast.h"
#include <vector>
#include <memory>

class Parser {
public:
    Parser(const std::vector<Token> &tokens);
    std::vector<std::unique_ptr<FuncDef>> parseCompUnit();

private:
    // 表达式相关
    std::unique_ptr<Expr> parseExpr();
    std::unique_ptr<Expr> parseLOrExpr();
    std::unique_ptr<Expr> parseLAndExpr();
    std::unique_ptr<Expr> parseRelExpr();
    std::unique_ptr<Expr> parseAddExpr();
    std::unique_ptr<Expr> parseMulExpr();
    std::unique_ptr<Expr> parseUnaryExpr();
    std::unique_ptr<Expr> parsePrimaryExpr();

    // 语句相关
    std::unique_ptr<Stmt> parseStmt();
    std::unique_ptr<Stmt> parseVarDecl();
    std::unique_ptr<Stmt> parseIfStmt();
    std::unique_ptr<Stmt> parseWhileStmt();
    std::unique_ptr<Stmt> parseBreakStmt();
    std::unique_ptr<Stmt> parseContinueStmt();
    std::unique_ptr<Stmt> parseReturnStmt();
    std::unique_ptr<Stmt> parseAssignOrExprStmt();
    std::unique_ptr<Block> parseBlock();

    // 函数定义
    std::unique_ptr<FuncDef> parseFuncDef();

    // 工具
    const Token &peek() const;
    const Token &advance();
    bool match(TokenType type);
    bool expect(TokenType type, const char *msg);

    const std::vector<Token> &tokens;
    size_t current = 0;
};
