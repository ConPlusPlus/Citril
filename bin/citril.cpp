#include "citril/interpreter.hpp"
#include "citril/lexer.hpp"
#include "citril/parser.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#if defined(_WIN32)
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace {

void load_library(citril::Interpreter& interpreter, const std::string& path) {
#if defined(_WIN32)
    HMODULE handle = LoadLibraryA(path.c_str());
    if (!handle) throw std::runtime_error("Could not load library: " + path);
    auto symbol = reinterpret_cast<citril::RegisterLibraryFn>(GetProcAddress(handle, "register_citril_library"));
#else
    void* handle = dlopen(path.c_str(), RTLD_NOW);
    if (!handle) throw std::runtime_error(std::string("Could not load library: ") + dlerror());
    auto symbol = reinterpret_cast<citril::RegisterLibraryFn>(dlsym(handle, "register_citril_library"));
#endif
    if (!symbol) throw std::runtime_error("Missing symbol register_citril_library in: " + path);
    symbol(interpreter);
}

} // namespace

int main(int argc, char** argv){
    if (argc < 2) {
        std::cerr << "Usage: citril [--lib path/to/lib]... <file>\n";
        return 1;
    }

    std::vector<std::string> libs;
    std::string script_path;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--lib") {
            if (i + 1 >= argc) { std::cerr << "--lib requires a path\n"; return 1; }
            libs.emplace_back(argv[++i]);
        } else {
            script_path = arg;
        }
    }

    if (script_path.empty()) { std::cerr << "Missing script path\n"; return 1; }

    std::ifstream in(script_path);
    if (!in) { std::cerr << "Could not open file\n"; return 1; }
    std::stringstream ss;
    ss << in.rdbuf();

    try {
        citril::Interpreter interpreter;
        for (const auto& lib : libs) load_library(interpreter, lib);

        citril::Lexer lex(ss.str());
        auto tokens = lex.scan_tokens();
        citril::Parser parser(tokens);
        auto statements = parser.parse();
        interpreter.interpret(statements);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
