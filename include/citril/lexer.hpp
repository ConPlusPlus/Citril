#pragma once
#include "citril/token.hpp"
#include <string>
#include <vector>

namespace citril {

class Lexer {
public:
    explicit Lexer(std::string source);
    std::vector<Token> scan_tokens();

private:
    std::string source_;
    std::vector<Token> tokens_;
    std::size_t start_ = 0;
    std::size_t current_ = 0;
    int line_ = 1;
    int column_ = 1;
    int token_column_ = 1;

    bool is_at_end() const;
    char advance();
    bool match(char expected);
    char peek() const;
    char peek_next() const;
    void add_token(TokenType type);
    void scan_token();
    void scan_string();
    void number();
    void identifier();
};

} // namespace citril
