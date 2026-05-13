#pragma once
#include <cmath>
#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

namespace citril {

class Interpreter;

struct Nil {};

class Callable {
public:
    virtual ~Callable() = default;
    virtual int arity() const = 0;
    virtual std::string name() const = 0;
    virtual std::variant<Nil, double, bool, std::string, std::shared_ptr<Callable>> call(
        Interpreter& interpreter,
        const std::vector<std::variant<Nil, double, bool, std::string, std::shared_ptr<Callable>>>& args
    ) = 0;
};

using Value = std::variant<Nil, double, bool, std::string, std::shared_ptr<Callable>>;

inline bool is_truthy(const Value& value) {
    if (std::holds_alternative<Nil>(value)) return false;
    if (auto b = std::get_if<bool>(&value)) return *b;
    return true;
}

inline std::string stringify(const Value& value) {
    if (std::holds_alternative<Nil>(value)) return "nil";
    if (auto n = std::get_if<double>(&value)) {
        std::ostringstream out;
        out << *n;
        auto s = out.str();
        if (s.size() > 2 && s.substr(s.size() - 2) == ".0") s.resize(s.size() - 2);
        return s;
    }
    if (auto b = std::get_if<bool>(&value)) return *b ? "true" : "false";
    if (auto s = std::get_if<std::string>(&value)) return *s;
    if (auto fn = std::get_if<std::shared_ptr<Callable>>(&value)) return "<fn " + (*fn)->name() + ">";
    return "nil";
}

inline bool values_equal(const Value& a, const Value& b) {
    if (a.index() != b.index()) return false;
    if (std::holds_alternative<Nil>(a)) return true;
    if (auto x = std::get_if<double>(&a)) return std::fabs(*x - std::get<double>(b)) < 1e-12;
    if (auto x = std::get_if<bool>(&a)) return *x == std::get<bool>(b);
    if (auto x = std::get_if<std::string>(&a)) return *x == std::get<std::string>(b);
    if (auto x = std::get_if<std::shared_ptr<Callable>>(&a)) return *x == std::get<std::shared_ptr<Callable>>(b);
    return false;
}

} // namespace citril
