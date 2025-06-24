#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"  // 一定要包含ast.h，保证UnaryExpr等定义可见
#include <stack>
#include <unordered_map>
#include <string>

enum class Type { Int, Void, Unknown };

struct Symbol {
    Type type = Type::Unknown;
    bool isFunction = false;
    std::vector<Type> paramTypes;
};

class SemanticAnalyzer {
public:
    void analyze(ASTNode *node);

private:
    std::stack<std::unordered_map<std::string, Symbol>> scopes;

    void enterScope();
    void exitScope();

    void declare(const std::string &name, const Symbol &symbol);
    Symbol lookup(const std::string &name);

    void analyzeFunc(FuncDef *func);
    void analyzeBlock(Block *block);
    void analyzeStmt(Stmt *stmt);

    Type analyzeExpr(Expr *expr);
    Type analyzeBinary(BinaryExpr *expr);
    Type analyzeCall(CallExpr *expr);
    Type analyzeVar(VarExpr *expr);
    Type analyzeNumber(NumberExpr *expr);
    Type analyzeUnary(UnaryExpr *expr);  // 这里UnaryExpr已定义

    void reportError(const std::string &msg);
};

#endif // SEMANTIC_H
