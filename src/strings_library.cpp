#include "citril/interpreter.hpp"
#include "citril/value.hpp"
#include <cctype>

extern "C" void register_citril_library(citril::Interpreter& interpreter) {
    interpreter.register_native("len", 1, [](const std::vector<citril::Value>& args) -> citril::Value {
        if (auto s = std::get_if<std::string>(&args[0])) return static_cast<double>(s->size());
        return 0.0;
    });

    interpreter.register_native("upper", 1, [](const std::vector<citril::Value>& args) -> citril::Value {
        if (auto s = std::get_if<std::string>(&args[0])) {
            std::string out = *s;
            for (char& c : out) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
            return out;
        }
        return std::string("");
    });
}
