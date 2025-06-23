#include "semantic.h"
#include "ast.h"
#include <iostream>
#include <stdexcept>

void SemanticAnalyzer::analyze(ASTNode *node) {
    if (auto *func = dynamic_cast<FuncDef *>(node)) {
        analyzeFunc(func);
    } else {
        reportError("Unsupported AST node in compilation unit");
    }
}

void SemanticAnalyzer::enterScope() {
    scopes.emplace();
}

void SemanticAnalyzer::exitScope() {
    if (scopes.empty()) {
        reportError("Scope underflow");
    }
    scopes.pop();
}

void SemanticAnalyzer::declare(const std::string &name, const Symbol &symbol) {
    if (scopes.empty()) {
        reportError("No active scope to declare symbol: " + name);
    }
    auto &currentScope = scopes.top();
    if (currentScope.count(name)) {
        reportError("Redefinition of symbol: " + name);
    }
    currentScope[name] = symbol;
}

Symbol SemanticAnalyzer::lookup(const std::string &name) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        if (it->count(name)) {
            return it->at(name);
        }
    }
    reportError("Undeclared identifier: " + name);
    return Symbol{Type::Unknown}; // 不会执行到这里，只是防止编译警告
}

void SemanticAnalyzer::analyzeFunc(FuncDef *func) {
    Symbol funcSym;
    funcSym.type = (func->retType == "int") ? Type::Int : Type::Void;
    funcSym.isFunction = true;
    funcSym.paramTypes.clear();

    for (auto &param : func->params) {
        // 目前只支持int参数类型
        funcSym.paramTypes.push_back(Type::Int);
    }

    // 函数名在外层作用域声明（可调整位置）
    if (scopes.empty()) enterScope(); // 保护性检查
    declare(func->name, funcSym);

    enterScope(); // 函数体新作用域
    for (auto &param : func->params) {
        declare(param.name, Symbol{Type::Int});
    }
    analyzeBlock(func->body.get());
    exitScope();
}

void SemanticAnalyzer::analyzeBlock(Block *block) {
    enterScope();
    for (auto &stmt : block->stmts) {
        analyzeStmt(stmt.get());
    }
    exitScope();
}

void SemanticAnalyzer::analyzeStmt(Stmt *stmt) {
    if (auto *exprStmt = dynamic_cast<ExprStmt *>(stmt)) {
        analyzeExpr(exprStmt->expr.get());
    } else if (auto *assign = dynamic_cast<AssignStmt *>(stmt)) {
        auto rhsType = analyzeExpr(assign->value.get());
        if (assign->isDecl) {
            declare(assign->varName, Symbol{rhsType});
        } else {
            auto existing = lookup(assign->varName);
            if (existing.type != rhsType) {
                reportError("Type mismatch in assignment to " + assign->varName);
            }
        }
    } else if (auto *ret = dynamic_cast<ReturnStmt *>(stmt)) {
        if (ret->value)
            analyzeExpr(ret->value.get());
    } else if (auto *blk = dynamic_cast<Block *>(stmt)) {
        analyzeBlock(blk);
    } else if (auto *ifStmt = dynamic_cast<IfStmt *>(stmt)) {
        analyzeExpr(ifStmt->cond.get());
        analyzeStmt(ifStmt->thenStmt.get());
        if (ifStmt->elseStmt) analyzeStmt(ifStmt->elseStmt.get());
    } else if (auto *whileStmt = dynamic_cast<WhileStmt *>(stmt)) {
        analyzeExpr(whileStmt->cond.get());
        analyzeStmt(whileStmt->body.get());
    } else {
        // break, continue等简单语句这里可忽略或加检查
    }
}

Type SemanticAnalyzer::analyzeExpr(Expr *expr) {
    if (auto *num = dynamic_cast<NumberExpr *>(expr)) {
        return analyzeNumber(num);
    } else if (auto *var = dynamic_cast<VarExpr *>(expr)) {
        return analyzeVar(var);
    } else if (auto *bin = dynamic_cast<BinaryExpr *>(expr)) {
        return analyzeBinary(bin);
    } else if (auto *call = dynamic_cast<CallExpr *>(expr)) {
        return analyzeCall(call);
    } else if (auto *unary = dynamic_cast<UnaryExpr *>(expr)) {
        return analyzeUnary(unary);
    } else {
        reportError("Unknown expression type");
        return Type::Unknown;
    }
}

Type SemanticAnalyzer::analyzeBinary(BinaryExpr *expr) {
    auto lhsType = analyzeExpr(expr->lhs.get());
    auto rhsType = analyzeExpr(expr->rhs.get());
    if (lhsType != Type::Int || rhsType != Type::Int) {
        reportError("Binary operator requires int operands");
    }
    return Type::Int;
}

Type SemanticAnalyzer::analyzeCall(CallExpr *expr) {
    auto sym = lookup(expr->callee);
    if (!sym.isFunction) {
        reportError(expr->callee + " is not a function");
    }
    if (expr->args.size() != sym.paramTypes.size()) {
        reportError("Argument count mismatch in call to " + expr->callee);
    }
    for (size_t i = 0; i < expr->args.size(); ++i) {
        Type argType = analyzeExpr(expr->args[i].get());
        if (argType != sym.paramTypes[i]) {
            reportError("Argument type mismatch in function call to " + expr->callee);
        }
    }
    return sym.type;
}

Type SemanticAnalyzer::analyzeVar(VarExpr *expr) {
    return lookup(expr->name).type;
}

Type SemanticAnalyzer::analyzeNumber(NumberExpr * /*expr*/) {
    return Type::Int;
}

Type SemanticAnalyzer::analyzeUnary(UnaryExpr *expr) {
    auto operandType = analyzeExpr(expr->operand.get());
    if (operandType != Type::Int) {
        reportError("Unary operator requires int operand");
    }
    return Type::Int;
}

void SemanticAnalyzer::reportError(const std::string &msg) {
    throw std::runtime_error("Semantic error: " + msg);
}