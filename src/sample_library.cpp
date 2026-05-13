#include "citril/interpreter.hpp"
#include "citril/value.hpp"

extern "C" void register_citril_library(citril::Interpreter& interpreter) {
    interpreter.register_native("len", 1, [](const std::vector<citril::Value>& args) -> citril::Value {
        if (auto s = std::get_if<std::string>(&args[0])) return static_cast<double>(s->size());
        return 0.0;
    });

    interpreter.register_native("type", 1, [](const std::vector<citril::Value>& args) -> citril::Value {
        if (std::holds_alternative<citril::Nil>(args[0])) return std::string("nil");
        if (std::holds_alternative<double>(args[0])) return std::string("number");
        if (std::holds_alternative<bool>(args[0])) return std::string("bool");
        if (std::holds_alternative<std::string>(args[0])) return std::string("string");
        return std::string("callable");
    });
}
