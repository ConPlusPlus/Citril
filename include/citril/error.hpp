#pragma once
#include <stdexcept>
#include <string>

namespace citril {

class CitrilError : public std::runtime_error {
public:
    explicit CitrilError(const std::string& message) : std::runtime_error(message) {}
};

class LexError : public CitrilError {
public:
    explicit LexError(const std::string& message) : CitrilError(message) {}
};

class ParseError : public CitrilError {
public:
    explicit ParseError(const std::string& message) : CitrilError(message) {}
};

class RuntimeError : public CitrilError {
public:
    explicit RuntimeError(const std::string& message) : CitrilError(message) {}
};

} // namespace citril
