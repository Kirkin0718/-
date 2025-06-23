#pragma once
#include <memory>
#include <string>
#include <vector>

// AST 节点基类
class ASTNode {
public:
    virtual ~ASTNode() = default;
};

// 表达式基类
class Expr : public ASTNode {
public:
    virtual ~Expr() = default;
};

// 字面量（整数）
class NumberExpr : public Expr {
public:
    int value;
    explicit NumberExpr(int val) : value(val) {}
};

// 变量引用
class VarExpr : public Expr {
public:
    std::string name;
    explicit VarExpr(const std::string &n) : name(n) {}
};

// 二元运算
class BinaryExpr : public Expr {
public:
    char op;
    std::unique_ptr<Expr> lhs;
    std::unique_ptr<Expr> rhs;
    
    BinaryExpr(char o, std::unique_ptr<Expr> l, std::unique_ptr<Expr> r)
        : op(o), lhs(std::move(l)), rhs(std::move(r)) {}
};

// 函数调用
class CallExpr : public Expr {
public:
    std::string callee;
    std::vector<std::unique_ptr<Expr>> args;

    CallExpr(const std::string &c, std::vector<std::unique_ptr<Expr>> a)
        : callee(c), args(std::move(a)) {}
};

// 语句基类
class Stmt : public ASTNode {
public:
    virtual ~Stmt() = default;
};

// 表达式语句
class ExprStmt : public Stmt {
public:
    std::unique_ptr<Expr> expr;
    explicit ExprStmt(std::unique_ptr<Expr> e) : expr(std::move(e)) {}
};

// 变量声明
class VarDecl : public Stmt {
public:
    std::string name;
    std::unique_ptr<Expr> init;

    VarDecl(const std::string &n, std::unique_ptr<Expr> i)
        : name(n), init(std::move(i)) {}
};

// 函数定义
class Function : public ASTNode {
public:
    std::string name;
    std::vector<std::string> params;
    std::vector<std::unique_ptr<Stmt>> body;

    Function(const std::string &n, 
             std::vector<std::string> p,
             std::vector<std::unique_ptr<Stmt>> b)
        : name(n), params(std::move(p)), body(std::move(b)) {}
};

// 程序（编译单元）
class Program : public ASTNode {
public:
    std::vector<std::unique_ptr<Function>> functions;
};