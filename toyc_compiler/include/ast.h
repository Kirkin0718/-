// === ast.h ===
#pragma once
#include <string>
#include <vector>
#include <memory>

struct ASTNode {
    virtual ~ASTNode() = default;
};

struct Expr : ASTNode {};
struct Stmt : ASTNode {};

struct Program : ASTNode {
    std::vector<std::unique_ptr<class FuncDef>> functions;
};

struct FuncDef : ASTNode {
    std::string retType, name;
    struct Param {
        std::string type, name;
    };
    std::vector<Param> params;
    std::unique_ptr<class Block> body;

    FuncDef(const std::string &rt, const std::string &n) : retType(rt), name(n) {}
};

struct Block : Stmt {
    std::vector<std::unique_ptr<Stmt>> stmts;
};

struct ReturnStmt : Stmt {
    std::unique_ptr<Expr> expr;
};

struct VarExpr : Expr {
    std::string name;
    VarExpr(std::string n) : name(std::move(n)) {}
};

struct NumberExpr : Expr {
    int value;
    NumberExpr(int v) : value(v) {}
};

struct UnaryExpr : Expr {
    std::string op;
    std::unique_ptr<Expr> operand;
    UnaryExpr(std::string o, std::unique_ptr<Expr> e) : op(std::move(o)), operand(std::move(e)) {}
};

struct BinaryExpr : Expr {
    std::string op;
    std::unique_ptr<Expr> lhs, rhs;
    BinaryExpr(std::string o, std::unique_ptr<Expr> l, std::unique_ptr<Expr> r)
        : op(std::move(o)), lhs(std::move(l)), rhs(std::move(r)) {}
};

struct CallExpr : Expr {
    std::string callee;
    std::vector<std::unique_ptr<Expr>> args;
    CallExpr(std::string c) : callee(std::move(c)) {}
};
