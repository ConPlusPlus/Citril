#pragma once
#include "citril/ast.hpp"
#include "citril/environment.hpp"
#include <memory>
#include <vector>

namespace citril {

class Interpreter {
public:
    Interpreter();
    void interpret(const std::vector<StmtPtr>& statements);

private:
    std::shared_ptr<Environment> globals_;
    std::shared_ptr<Environment> environment_;

    void execute(const StmtPtr& stmt);
    void execute_block(const std::vector<StmtPtr>& statements, std::shared_ptr<Environment> env);
    Value evaluate(const ExprPtr& expr);
};

} // namespace citril
