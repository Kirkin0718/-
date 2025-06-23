#pragma once
#include "ast.h"
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include <stack>
#include <stdexcept>

// 简单类型系统
enum class Type {
    Int,
    Void,
    Unknown
};

// 符号信息（变量或函数）
struct Symbol {
    Type type;
    bool isFunction = false;
    std::vector<Type> paramTypes; // 如果是函数，记录参数类型
};

// 语义分析器
class SemanticAnalyzer {
public:
    void analyze(ASTNode *node);  // 分析整个编译单元

private:
    // 当前作用域的符号表栈
    std::stack<std::unordered_map<std::string, Symbol>> scopes;

    void enterScope();
    void exitScope();

    void declare(const std::string &name, const Symbol &symbol);
    Symbol lookup(const std::string &name);

    void analyzeFunc(Function *func);
    void analyzeBlock(/*如果你有Block类，传Block*，否则用合适类型*/);
    void analyzeStmt(Stmt *stmt);
    Type analyzeExpr(Expr *expr);

    Type analyzeBinary(BinaryExpr *expr);
    Type analyzeCall(CallExpr *expr);
    Type analyzeVar(VarExpr *expr);
    Type analyzeNumber(NumberExpr *expr);
    Type analyzeUnary(UnaryExpr *expr);

    void reportError(const std::string &msg);
};
