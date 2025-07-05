// test_semantic.cpp
#include "../include/semantic.h"
#include "../include/ast.h"

#include <iostream>
#include <memory>
#include <cassert>

void test_simple_function() {
    auto func = std::make_unique<FuncDef>("int", "main");
    func->params = {};  // 无参数

    auto block = std::make_unique<Block>();
    auto varDecl = std::make_unique<VarDeclStmt>("int", "a",
                    std::make_unique<NumberExpr>(1));
    auto assign = std::make_unique<AssignStmt>("a",
                    std::make_unique<NumberExpr>(2));

    block->stmts.push_back(std::move(varDecl));
    block->stmts.push_back(std::move(assign));
    func->body = std::move(block);

    std::vector<std::unique_ptr<FuncDef>> funcs;
    funcs.push_back(std::move(func));

    SemanticAnalyzer analyzer;
    analyzer.analyze(funcs);

    std::cout << "test_simple_function passed\n";
}

void test_undeclared_variable() {
    auto func = std::make_unique<FuncDef>("int", "main");
    func->params = {};

    auto block = std::make_unique<Block>();
    auto assign = std::make_unique<AssignStmt>("x", 
                    std::make_unique<NumberExpr>(10));  // x 未声明

    block->stmts.push_back(std::move(assign));
    func->body = std::move(block);

    std::vector<std::unique_ptr<FuncDef>> funcs;
    funcs.push_back(std::move(func));

    SemanticAnalyzer analyzer;
    analyzer.analyze(funcs);

    std::cout << "test_undeclared_variable passed (should print an error above)\n";
}

void test_duplicate_variable() {
    auto func = std::make_unique<FuncDef>("int", "main");
    func->params = {};

    auto block = std::make_unique<Block>();
    auto varDecl1 = std::make_unique<VarDeclStmt>("int", "a",
                    std::make_unique<NumberExpr>(1));
    auto varDecl2 = std::make_unique<VarDeclStmt>("int", "a",
                    std::make_unique<NumberExpr>(2));  // 重复声明

    block->stmts.push_back(std::move(varDecl1));
    block->stmts.push_back(std::move(varDecl2));
    func->body = std::move(block);

    std::vector<std::unique_ptr<FuncDef>> funcs;
    funcs.push_back(std::move(func));

    SemanticAnalyzer analyzer;
    analyzer.analyze(funcs);

    std::cout << "test_duplicate_variable passed (should print an error above)\n";
}

int main() {
    test_simple_function();
    test_undeclared_variable();
    test_duplicate_variable();
    std::cout << "All semantic tests done.\n";
    return 0;
}
