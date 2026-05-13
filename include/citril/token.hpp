#pragma once
#include <string>

namespace citril {

enum class TokenType {
    // Single-character tokens
    LeftParen, RightParen, LeftBrace, RightBrace,
    Comma, Dot, Minus, Plus, Semicolon, Slash, Star, Percent,

    // One or two character tokens
    Bang, BangEqual,
    Equal, EqualEqual,
    Greater, GreaterEqual,
    Less, LessEqual,

    // Literals
    Identifier, String, Number,

    // Keywords
    And, Else, False, Fn, If, Let, LocalVar, Nil, Or, Return, True, Var, While,

    EndOfFile
};

struct Token {
    TokenType type{};
    std::string lexeme;
    int line = 1;
    int column = 1;
};

std::string token_type_name(TokenType type);

} // namespace citril
