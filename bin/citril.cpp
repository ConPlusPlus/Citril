#include "citril/interpreter.hpp"
#include "citril/lexer.hpp"
#include "citril/parser.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace {
bool is_dynamic_library_file(const std::filesystem::path& path) {
#if defined(_WIN32)
    return path.extension() == ".dll";
#elif defined(__APPLE__)
    return path.extension() == ".dylib";
#else
    return path.extension() == ".so";
#endif
}
}

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

    std::ifstream in(script_path);
    if (!in) { std::cerr << "Could not open file\n"; return 1; }
    std::stringstream ss;
    ss << in.rdbuf();

    try {
        citril::Interpreter interpreter;
        if (std::filesystem::exists("libraries")) {
            for (const auto& entry : std::filesystem::directory_iterator("libraries")) {
                if (entry.is_regular_file() && is_dynamic_library_file(entry.path())) interpreter.load_library_path(entry.path().string());
            }
        }
        for (const auto& lib : libs) interpreter.load_library_path(lib);

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
