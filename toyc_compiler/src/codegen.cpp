// codegen.cpp
#include "codegen.h"
#include <cassert>

CodeGen::CodeGen(std::ostream &out) : out(out) {}

void CodeGen::emit(const std::string &code) {
    out << "\t" << code << "\n";
}

std::string CodeGen::newLabel(const std::string &base) {
    return base + "_" + std::to_string(labelCount++);
}

void CodeGen::generate(Program *program) {
    for (auto &func : program->functions) {
        genFunc(func.get());
    }
}

void CodeGen::genFunc(Function *func) {
    out << func->name << ":\n";
    emit("addi sp, sp, -128");    // 分配固定栈空间（可根据需要调整）
    emit("sw ra, 124(sp)");       // 保存返回地址

    localVarOffset.clear();
    int offset = 0;

    // 参数入栈
    for (size_t i = 0; i < func->params.size(); ++i) {
        localVarOffset[func->params[i]] = offset;
        emit("sw a" + std::to_string(i) + ", " + std::to_string(offset) + "(sp)");
        offset += 4;
    }

    // 生成函数体代码
    for (auto &stmt : func->body) {
        genStmt(stmt.get());
    }

    emit("lw ra, 124(sp)");       // 恢复返回地址
    emit("addi sp, sp, 128");     // 释放栈空间
    emit("ret");
}

std::string CodeGen::genExpr(Expr *expr) {
    if (auto num = dynamic_cast<NumberExpr *>(expr)) {
        emit("li a0, " + std::to_string(num->value));
        return "a0";
    } else if (auto var = dynamic_cast<VarExpr *>(expr)) {
        assert(localVarOffset.count(var->name) && "变量未声明");
        int offset = localVarOffset[var->name];
        emit("lw a0, " + std::to_string(offset) + "(sp)");
        return "a0";
    } else if (auto bin = dynamic_cast<BinaryExpr *>(expr)) {
        genExpr(bin->lhs.get());
        emit("mv t0, a0");
        genExpr(bin->rhs.get());

        switch (bin->op) {
            case '+': emit("add a0, t0, a0"); break;
            case '-': emit("sub a0, t0, a0"); break;
            case '*': emit("mul a0, t0, a0"); break;
            case '/': emit("div a0, t0, a0"); break;
            case '%': emit("rem a0, t0, a0"); break;
            case '<': emit("slt a0, t0, a0"); break;
            case '>': emit("sgt a0, t0, a0"); break;
            case '=': // == 操作
                emit("xor a0, t0, a0");
                emit("seqz a0, a0");
                break;
            case '!': // != 操作
                emit("xor a0, t0, a0");
                emit("snez a0, a0");
                break;
            default:
                assert(false && "不支持的二元运算符");
        }
        return "a0";
    }

    assert(false && "未知表达式类型");
    return "";
}

void CodeGen::genStmt(Stmt *stmt) {
    if (auto exprStmt = dynamic_cast<ExprStmt *>(stmt)) {
        genExpr(exprStmt->expr.get());
    } else if (auto retStmt = dynamic_cast<ReturnStmt *>(stmt)) {
        if (retStmt->value)
            genExpr(retStmt->value.get());
        emit("ret");
    } else if (auto assignStmt = dynamic_cast<AssignStmt *>(stmt)) {
        genExpr(assignStmt->value.get());
        assert(localVarOffset.count(assignStmt->name) && "赋值变量未声明");
        int offset = localVarOffset[assignStmt->name];
        emit("sw a0, " + std::to_string(offset) + "(sp)");
    } else if (auto block = dynamic_cast<Block *>(stmt)) {
        genBlock(block);
    } else {
        assert(false && "不支持的语句类型");
    }
}

void CodeGen::genBlock(Block *block) {
    for (auto &stmt : block->stmts) {
        genStmt(stmt.get());
    }
}
