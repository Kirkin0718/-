#include <iostream>
#include <sstream>
#include <memory>
#include <vector>

#include "codegen.h"
#include "ast.h"

int main() {
    // 创建一个输出字符串流，用于捕获代码生成输出
    std::ostringstream oss;

    // 创建CodeGen对象，传入输出流
    CodeGen codegen(oss);

    // 构造一个简单的函数AST：
    // int main() { return 42; }
    auto func = std::make_unique<FuncDef>("int", "main");

    // 创建返回语句：return 42;
    auto retStmt = std::make_unique<ReturnStmt>();
    retStmt->expr = std::make_unique<NumberExpr>(42);

    // 创建函数体块Block并添加return语句
    func->body = std::make_unique<Block>();
    func->body->stmts.push_back(std::move(retStmt));

    // 将函数放入vector
    std::vector<std::unique_ptr<FuncDef>> funcs;
    funcs.push_back(std::move(func));

    // 调用CodeGen生成代码
    codegen.generate(funcs);

    // 输出生成的代码
    std::cout << "Generated code:\n" << oss.str() << std::endl;

    return 0;
}
