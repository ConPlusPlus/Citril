#pragma once
#include "citril/error.hpp"
#include "citril/token.hpp"
#include "citril/value.hpp"
#include <memory>
#include <string>
#include <unordered_map>

namespace citril {

class Environment : public std::enable_shared_from_this<Environment> {
public:
    explicit Environment(std::shared_ptr<Environment> enclosing = nullptr);

    void define(const std::string& name, Value value);
    Value get(const Token& name) const;
    void assign(const Token& name, Value value);

private:
    std::unordered_map<std::string, Value> values_;
    std::shared_ptr<Environment> enclosing_;
};

} // namespace citril
