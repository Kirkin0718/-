#include "semantic.h"
#include "ast.h"
#include <stdexcept>

void SemanticAnalyzer::analyze(ASTNode *node) {
    if (auto func = dynamic_cast<FuncDef*>(node)) {
        analyzeFunc(func);
    } else {
        reportError("Unsupported AST node");
    }
}

void SemanticAnalyzer::enterScope() {
    scopes.emplace();
}

void SemanticAnalyzer::exitScope() {
    if (scopes.empty()) reportError("Scope stack underflow");
    scopes.pop();
}

void SemanticAnalyzer::declare(const std::string &name, const Symbol &symbol) {
    if (scopes.empty()) enterScope();
    auto &scope = scopes.top();
    if (scope.count(name)) {
        reportError("Redefinition of " + name);
    }
    scope[name] = symbol;
}

Symbol SemanticAnalyzer::lookup(const std::string &name) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        if (it->count(name)) return it->at(name);
    }
    reportError("Undeclared identifier: " + name);
    return Symbol{Type::Unknown};
}

void SemanticAnalyzer::analyzeFunc(FuncDef *func) {
    Symbol sym;
    sym.type = func->retType == "int" ? Type::Int : Type::Void;
    sym.isFunction = true;
    for (auto &param : func->params) {
        sym.paramTypes.push_back(Type::Int);
    }
    declare(func->name, sym);

    enterScope();
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
    if (auto exprStmt = dynamic_cast<ExprStmt*>(stmt)) {
        analyzeExpr(exprStmt->expr.get());
    } else if (auto assign = dynamic_cast<AssignStmt*>(stmt)) {
        auto rhsType = analyzeExpr(assign->value.get());
        if (assign->isDecl) {
            declare(assign->varName, Symbol{rhsType});
        } else {
            auto sym = lookup(assign->varName);
            if (sym.type != rhsType)
                reportError("Type mismatch in assignment");
        }
    } else if (auto ret = dynamic_cast<ReturnStmt*>(stmt)) {
        if (ret->value)
            analyzeExpr(ret->value.get());
    } else if (auto block = dynamic_cast<Block*>(stmt)) {
        analyzeBlock(block);
    } else if (auto ifStmt = dynamic_cast<IfStmt*>(stmt)) {
        analyzeExpr(ifStmt->cond.get());
        analyzeStmt(ifStmt->thenStmt.get());
        if (ifStmt->elseStmt) analyzeStmt(ifStmt->elseStmt.get());
    } else if (auto whileStmt = dynamic_cast<WhileStmt*>(stmt)) {
        analyzeExpr(whileStmt->cond.get());
        analyzeStmt(whileStmt->body.get());
    } else {
        // 其他语句如break, continue可忽略基础处理
    }
}

Type SemanticAnalyzer::analyzeExpr(Expr *expr) {
    if (auto num = dynamic_cast<NumberExpr*>(expr)) {
        return Type::Int;
    } else if (auto var = dynamic_cast<VarExpr*>(expr)) {
        return lookup(var->name).type;
    } else if (auto bin = dynamic_cast<BinaryExpr*>(expr)) {
        auto lhsType = analyzeExpr(bin->lhs.get());
        auto rhsType = analyzeExpr(bin->rhs.get());
        if (lhsType != Type::Int || rhsType != Type::Int) {
            reportError("Binary operator requires int operands");
        }
        return Type::Int;
    } else if (auto call = dynamic_cast<CallExpr*>(expr)) {
        auto sym = lookup(call->callee);
        if (!sym.isFunction) reportError(call->callee + " is not a function");
        if (call->args.size() != sym.paramTypes.size()) reportError("Argument count mismatch");
        for (size_t i = 0; i < call->args.size(); i++) {
            auto argType = analyzeExpr(call->args[i].get());
            if (argType != sym.paramTypes[i]) reportError("Argument type mismatch");
        }
        return sym.type;
    } else if (auto unary = dynamic_cast<UnaryExpr*>(expr)) {
        auto type = analyzeExpr(unary->operand.get());
        if (type != Type::Int) reportError("Unary operator requires int operand");
        return Type::Int;
    } else {
        reportError("Unknown expression");
        return Type::Unknown;
    }
}

void SemanticAnalyzer::reportError(const std::string &msg) {
    throw std::runtime_error("Semantic error: " + msg);
}