#pragma once
#include "citril/ast.hpp"
#include "citril/token.hpp"
#include <vector>

namespace citril {

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);
    std::vector<StmtPtr> parse();

private:
    std::vector<Token> tokens_;
    std::size_t current_ = 0;
    int block_depth_ = 0;

    StmtPtr declaration();
    StmtPtr var_declaration(bool require_local_scope);
    StmtPtr statement();
    StmtPtr include_statement();
    StmtPtr if_statement();
    StmtPtr while_statement();
    StmtPtr block_statement();
    StmtPtr expression_statement();

    ExprPtr expression();
    ExprPtr assignment();
    ExprPtr logic_or();
    ExprPtr logic_and();
    ExprPtr equality();
    ExprPtr comparison();
    ExprPtr term();
    ExprPtr factor();
    ExprPtr unary();
    ExprPtr call();
    ExprPtr primary();

    bool match(std::initializer_list<TokenType> types);
    bool check(TokenType type) const;
    const Token& advance();
    bool is_at_end() const;
    const Token& peek() const;
    const Token& previous() const;
    const Token& consume(TokenType type, const std::string& message);
    void consume_optional_semicolon();
};

} // namespace citril
