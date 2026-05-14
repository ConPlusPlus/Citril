#include "citril/interpreter.hpp"
#include "citril/value.hpp"
#include <chrono>

// Library: time_library
// Called when Citril loads this library.
extern "C" void register_citril_library(citril::Interpreter& interpreter) {
    interpreter.register_native("clock", 0, [](const std::vector<citril::Value>&) -> citril::Value {
        using namespace std::chrono;
        auto now = high_resolution_clock::now().time_since_epoch();
        return duration<double>(now).count();
    });
}
