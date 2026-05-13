#include "citril/interpreter.hpp"
#include "citril/value.hpp"
#include <iostream>

extern "C" void register_citril_library(citril::Interpreter& interpreter) {
    interpreter.register_native("print", 1, [](const std::vector<citril::Value>& args) -> citril::Value {
        std::cout << citril::stringify(args[0]) << "\n";
        return citril::Nil{};
    });
}
