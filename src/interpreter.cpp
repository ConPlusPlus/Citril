#include "citril/interpreter.hpp"
#include "citril/error.hpp"
#include "citril/value.hpp"
#include <cmath>
#include <filesystem>

#if defined(_WIN32)
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace citril {

NativeFunction::NativeFunction(std::string fn_name, int fn_arity, NativeImpl impl)
    : name_(std::move(fn_name)), arity_(fn_arity), impl_(std::move(impl)) {}
int NativeFunction::arity() const { return arity_; }
std::string NativeFunction::name() const { return name_; }
Value NativeFunction::call(Interpreter&, const std::vector<Value>& args) { return impl_(args); }

Interpreter::Interpreter() : globals_(std::make_shared<Environment>()), environment_(globals_) {}

void Interpreter::register_native(const std::string& name, int arity, NativeFunction::NativeImpl impl) {
    globals_->define(name, std::make_shared<NativeFunction>(name, arity, std::move(impl)));
}

void Interpreter::load_library_path(const std::string& path) {
#if defined(_WIN32)
    HMODULE handle = LoadLibraryA(path.c_str());
    if (!handle) throw RuntimeError("Could not load library: " + path);
    auto symbol = reinterpret_cast<RegisterLibraryFn>(GetProcAddress(handle, "register_citril_library"));
#else
    void* handle = dlopen(path.c_str(), RTLD_NOW);
    if (!handle) throw RuntimeError(std::string("Could not load library: ") + dlerror());
    auto symbol = reinterpret_cast<RegisterLibraryFn>(dlsym(handle, "register_citril_library"));
#endif
    if (!symbol) throw RuntimeError("Missing symbol register_citril_library in: " + path);
    symbol(*this);
}

void Interpreter::load_library_name(const std::string& name) {
    std::vector<std::filesystem::path> candidates;
    auto add_candidate = [&](const std::string& filename) {
        candidates.push_back(std::filesystem::path("libraries") / filename);
        candidates.push_back(std::filesystem::path("..") / "libraries" / filename);
    };
#if defined(_WIN32)
    add_candidate(name + ".dll");
#elif defined(__APPLE__)
    add_candidate(name + ".dylib");
    add_candidate("lib" + name + ".dylib");
#else
    add_candidate(name + ".so");
    add_candidate("lib" + name + ".so");
#endif
    for (const auto& c : candidates) { if (std::filesystem::exists(c)) { load_library_path(c.string()); return; } }
    throw RuntimeError("Could not find library: " + name);
}

void Interpreter::interpret(const std::vector<StmtPtr>& statements) { for (const auto& s : statements) execute(s); }
void Interpreter::execute(const StmtPtr& s) {
    switch (s->kind) {
        case Stmt::Kind::Expression: evaluate(s->expression); break;
        case Stmt::Kind::Let: environment_->define(s->token.lexeme, evaluate(s->initializer)); break;
        case Stmt::Kind::Block: execute_block(s->statements, std::make_shared<Environment>(environment_)); break;
        case Stmt::Kind::If: if (is_truthy(evaluate(s->condition))) execute(s->then_branch); else if (s->else_branch) execute(s->else_branch); break;
        case Stmt::Kind::While: while (is_truthy(evaluate(s->condition))) execute(s->then_branch); break;
        case Stmt::Kind::Include: load_library_name(s->module_name); break;
        default: throw RuntimeError("Unsupported statement kind");
    }
}
void Interpreter::execute_block(const std::vector<StmtPtr>& st, std::shared_ptr<Environment> env) { auto previous = environment_; environment_ = std::move(env); for (const auto& s : st) execute(s); environment_ = previous; }
Value Interpreter::evaluate(const ExprPtr& e) { /* unchanged */
    switch (e->kind) {
        case Expr::Kind::Literal: return e->literal;
        case Expr::Kind::Variable: return environment_->get(e->token);
        case Expr::Kind::Assign: { auto value = evaluate(e->right); environment_->assign(e->token, value); return value; }
        case Expr::Kind::Unary: { auto right = evaluate(e->right); if (e->token.type == TokenType::Minus) return -std::get<double>(right); if (e->token.type == TokenType::Bang) return !is_truthy(right); break; }
        case Expr::Kind::Logical: { auto left = evaluate(e->left); if (e->token.type == TokenType::Or) { if (is_truthy(left)) return left; } else if (!is_truthy(left)) { return left; } return evaluate(e->right); }
        case Expr::Kind::Binary: { auto left = evaluate(e->left); auto right = evaluate(e->right); switch (e->token.type) { case TokenType::Plus: if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) return std::get<double>(left) + std::get<double>(right); return stringify(left) + stringify(right); case TokenType::Minus: return std::get<double>(left) - std::get<double>(right); case TokenType::Star: return std::get<double>(left) * std::get<double>(right); case TokenType::Slash: return std::get<double>(left) / std::get<double>(right); case TokenType::Percent: return std::fmod(std::get<double>(left), std::get<double>(right)); case TokenType::Greater: return std::get<double>(left) > std::get<double>(right); case TokenType::GreaterEqual: return std::get<double>(left) >= std::get<double>(right); case TokenType::Less: return std::get<double>(left) < std::get<double>(right); case TokenType::LessEqual: return std::get<double>(left) <= std::get<double>(right); case TokenType::EqualEqual: return values_equal(left, right); case TokenType::BangEqual: return !values_equal(left, right); default: break; } break; }
        case Expr::Kind::Call: { auto callee = evaluate(e->callee); auto fn = std::get<std::shared_ptr<Callable>>(callee); std::vector<Value> args; for (const auto& arg : e->arguments) args.push_back(evaluate(arg)); if (static_cast<int>(args.size()) != fn->arity()) throw RuntimeError("Wrong arity"); return fn->call(*this, args); }
        default: break;
    }
    throw RuntimeError("Invalid expression");
}

} // namespace citril
