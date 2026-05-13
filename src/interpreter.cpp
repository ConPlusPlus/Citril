#include "citril/interpreter.hpp"
#include "citril/error.hpp"
#include "citril/value.hpp"
#include <cmath>
#include <iostream>

namespace citril {

NativeFunction::NativeFunction(std::string fn_name, int fn_arity, NativeImpl impl)
    : name_(std::move(fn_name)), arity_(fn_arity), impl_(std::move(impl)) {}

int NativeFunction::arity() const { return arity_; }
std::string NativeFunction::name() const { return name_; }
Value NativeFunction::call(Interpreter&, const std::vector<Value>& args) { return impl_(args); }

Interpreter::Interpreter() : globals_(std::make_shared<Environment>()), environment_(globals_) {}
Interpreter::Interpreter() : globals_(std::make_shared<Environment>()), environment_(globals_) {
    register_native("print", 1, [](const std::vector<Value>& args) {
        std::cout << stringify(args[0]) << "\n";
        return Value{Nil{}};
    });
}

void Interpreter::register_native(const std::string& name, int arity, NativeFunction::NativeImpl impl) {
    globals_->define(name, std::make_shared<NativeFunction>(name, arity, std::move(impl)));
}

void Interpreter::interpret(const std::vector<StmtPtr>& statements) { for (const auto& s : statements) execute(s); }

void Interpreter::execute(const StmtPtr& s) {
    switch (s->kind) {
        case Stmt::Kind::Expression: evaluate(s->expression); break;
        case Stmt::Kind::Let: environment_->define(s->token.lexeme, evaluate(s->initializer)); break;
        case Stmt::Kind::Block: execute_block(s->statements, std::make_shared<Environment>(environment_)); break;
        case Stmt::Kind::If: if (is_truthy(evaluate(s->condition))) execute(s->then_branch); else if (s->else_branch) execute(s->else_branch); break;
        case Stmt::Kind::While: while (is_truthy(evaluate(s->condition))) execute(s->then_branch); break;
        default: throw RuntimeError("Unsupported statement kind");
    }
}

void Interpreter::execute_block(const std::vector<StmtPtr>& st, std::shared_ptr<Environment> env) {
    auto previous = environment_;
    environment_ = std::move(env);
    for (const auto& s : st) execute(s);
    environment_ = previous;
}

Value Interpreter::evaluate(const ExprPtr& e) {
    switch (e->kind) {
        case Expr::Kind::Literal: return e->literal;
        case Expr::Kind::Variable: return environment_->get(e->token);
        case Expr::Kind::Assign: {
            auto value = evaluate(e->right);
            environment_->assign(e->token, value);
            return value;
        }
        case Expr::Kind::Unary: {
            auto right = evaluate(e->right);
            if (e->token.type == TokenType::Minus) return -std::get<double>(right);
            if (e->token.type == TokenType::Bang) return !is_truthy(right);
            break;
        }
        case Expr::Kind::Logical: {
            auto left = evaluate(e->left);
            if (e->token.type == TokenType::Or) {
                if (is_truthy(left)) return left;
            } else if (!is_truthy(left)) {
                return left;
            }
            return evaluate(e->right);
        }
        case Expr::Kind::Binary: {
            auto left = evaluate(e->left);
            auto right = evaluate(e->right);
            switch (e->token.type) {
                case TokenType::Plus:
                    if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right))
                        return std::get<double>(left) + std::get<double>(right);
                    return stringify(left) + stringify(right);
                case TokenType::Minus: return std::get<double>(left) - std::get<double>(right);
                case TokenType::Star: return std::get<double>(left) * std::get<double>(right);
                case TokenType::Slash: return std::get<double>(left) / std::get<double>(right);
                case TokenType::Percent: return std::fmod(std::get<double>(left), std::get<double>(right));
                case TokenType::Greater: return std::get<double>(left) > std::get<double>(right);
                case TokenType::GreaterEqual: return std::get<double>(left) >= std::get<double>(right);
                case TokenType::Less: return std::get<double>(left) < std::get<double>(right);
                case TokenType::LessEqual: return std::get<double>(left) <= std::get<double>(right);
                case TokenType::EqualEqual: return values_equal(left, right);
                case TokenType::BangEqual: return !values_equal(left, right);
                default: break;
            }
            break;
        }
        case Expr::Kind::Call: {
            auto callee = evaluate(e->callee);
            auto fn = std::get<std::shared_ptr<Callable>>(callee);
            std::vector<Value> args;
            for (const auto& arg : e->arguments) args.push_back(evaluate(arg));
            if (static_cast<int>(args.size()) != fn->arity()) throw RuntimeError("Wrong arity");
            return fn->call(*this, args);
        }
        default: break;
    }
    throw RuntimeError("Invalid expression");
}

} // namespace citril
#include <chrono>
#include <iostream>

namespace citril {
class NativePrint : public Callable { public: int arity() const override { return 1; } std::string name() const override { return "print"; } Value call(Interpreter&, const std::vector<Value>& args) override { std::cout << stringify(args[0]) << "\n"; return Nil{}; } };
Interpreter::Interpreter() : globals_(std::make_shared<Environment>()), environment_(globals_) { globals_->define("print", std::make_shared<NativePrint>()); }
void Interpreter::interpret(const std::vector<StmtPtr>& statements){ for(auto&s:statements) execute(s); }
void Interpreter::execute(const StmtPtr& s){ switch(s->kind){ case Stmt::Kind::Expression: evaluate(s->expression); break; case Stmt::Kind::Let: environment_->define(s->token.lexeme,evaluate(s->initializer)); break; case Stmt::Kind::Block: execute_block(s->statements,std::make_shared<Environment>(environment_)); break; case Stmt::Kind::If: if(is_truthy(evaluate(s->condition))) execute(s->then_branch); else if(s->else_branch) execute(s->else_branch); break; case Stmt::Kind::While: while(is_truthy(evaluate(s->condition))) execute(s->then_branch); break; default: throw RuntimeError("Unsupported statement kind"); } }
void Interpreter::execute_block(const std::vector<StmtPtr>& st,std::shared_ptr<Environment> env){ auto prev=environment_; environment_=env; for(auto&s:st) execute(s); environment_=prev; }
Value Interpreter::evaluate(const ExprPtr& e){ switch(e->kind){ case Expr::Kind::Literal: return e->literal; case Expr::Kind::Variable: return environment_->get(e->token); case Expr::Kind::Assign: { auto v=evaluate(e->right); environment_->assign(e->token,v); return v; } case Expr::Kind::Unary: { auto r=evaluate(e->right); if(e->token.type==TokenType::Minus) return -std::get<double>(r); if(e->token.type==TokenType::Bang) return !is_truthy(r); } break; case Expr::Kind::Logical:{ auto l=evaluate(e->left); if(e->token.type==TokenType::Or){ if(is_truthy(l)) return l; } else if(!is_truthy(l)) return l; return evaluate(e->right);} case Expr::Kind::Binary:{ auto l=evaluate(e->left), r=evaluate(e->right); switch(e->token.type){ case TokenType::Plus: if(std::holds_alternative<double>(l)&&std::holds_alternative<double>(r)) return std::get<double>(l)+std::get<double>(r); return stringify(l)+stringify(r); case TokenType::Minus: return std::get<double>(l)-std::get<double>(r); case TokenType::Star: return std::get<double>(l)*std::get<double>(r); case TokenType::Slash: return std::get<double>(l)/std::get<double>(r); case TokenType::Percent: return std::fmod(std::get<double>(l),std::get<double>(r)); case TokenType::Greater: return std::get<double>(l)>std::get<double>(r); case TokenType::GreaterEqual: return std::get<double>(l)>=std::get<double>(r); case TokenType::Less: return std::get<double>(l)<std::get<double>(r); case TokenType::LessEqual: return std::get<double>(l)<=std::get<double>(r); case TokenType::EqualEqual: return values_equal(l,r); case TokenType::BangEqual: return !values_equal(l,r); default: break; }} break; case Expr::Kind::Call:{ auto cal=evaluate(e->callee); auto fn=std::get<std::shared_ptr<Callable>>(cal); std::vector<Value> args; for(auto&a:e->arguments) args.push_back(evaluate(a)); if((int)args.size()!=fn->arity()) throw RuntimeError("Wrong arity"); return fn->call(*this,args);} default: break; } throw RuntimeError("Invalid expression"); }
}
