#pragma once
#include "ast.h"
#include <ostream>
#include <unordered_map>
#include <string>

// 前置声明，避免未识别错误
class Block;
class FuncDef;
class ReturnStmt;
class AssignStmt;
class DeclStmt;
class IfStmt;
class WhileStmt;

class CodeGen {
public:
    explicit CodeGen(std::ostream &output);

    void generate(ASTNode *node); // 生成整个程序的汇编

private:
    std::ostream &out;
    int labelCount = 0;
    int tempReg = 10; // x10 开始分配临时寄存器（a0）

    std::unordered_map<std::string, int> varOffsets; // 局部变量到栈偏移
    int stackOffset = 0;

    void genExpr(Expr *expr);             // 生成表达式
    void genStmt(Stmt *stmt);             // 生成语句
    void genBlock(Block *block);          // 生成语句块
    void genFuncDef(FuncDef *func);       // 生成函数
    void genReturn(ReturnStmt *stmt);     // 生成 return
    void genBinary(BinaryExpr *expr);     // 生成二元表达式
    void genNumber(NumberExpr *expr);     // 生成常数
    void genVar(VarExpr *expr);           // 生成变量
    void genAssign(AssignStmt *stmt);     // 赋值语句
    void genDecl(DeclStmt *stmt);         // 声明并初始化变量
    void genCall(CallExpr *expr);         // 函数调用
    void genExprStmt(ExprStmt *stmt);     // 表达式语句
    void genIf(IfStmt *stmt);             // 生成 if 语句（可选）
    void genWhile(WhileStmt *stmt);       // 生成 while 语句（可选）

    std::string newLabel(const std::string &prefix); // 生成新标签
    void push();                         // 分配新栈变量（用于局部变量）
};
