#pragma once
#include "citril/token.hpp"
#include "citril/value.hpp"
#include <memory>
#include <vector>

namespace citril {

struct Expr;
struct Stmt;
using ExprPtr = std::shared_ptr<Expr>;
using StmtPtr = std::shared_ptr<Stmt>;

struct Expr {
    enum class Kind { Literal, Variable, Assign, Unary, Binary, Logical, Call } kind;
    Token token;
    Value literal = Nil{};
    ExprPtr left;
    ExprPtr right;
    ExprPtr callee;
    std::vector<ExprPtr> arguments;

    explicit Expr(Kind kind) : kind(kind) {}
};

struct Stmt {
    enum class Kind { Expression, Let, Block, If, While, Function, Return } kind;
    Token token;
    ExprPtr expression;
    ExprPtr initializer;
    ExprPtr condition;
    StmtPtr then_branch;
    StmtPtr else_branch;
    std::vector<StmtPtr> statements;
    std::vector<Token> params;

    explicit Stmt(Kind kind) : kind(kind) {}
};

} // namespace citril
