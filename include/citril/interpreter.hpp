#pragma once
#include "citril/ast.hpp"
#include "citril/environment.hpp"
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace citril {

class Interpreter;

class NativeFunction : public Callable {
public:
    using NativeImpl = std::function<Value(const std::vector<Value>&)>;

    NativeFunction(std::string fn_name, int fn_arity, NativeImpl impl);
    int arity() const override;
    std::string name() const override;
    Value call(Interpreter& interpreter, const std::vector<Value>& args) override;

private:
    std::string name_;
    int arity_;
    NativeImpl impl_;
};

class Interpreter {
public:
    Interpreter();
    void interpret(const std::vector<StmtPtr>& statements);

    void register_native(const std::string& name, int arity, NativeFunction::NativeImpl impl);
    void load_library_path(const std::string& path);
    void load_library_name(const std::string& name);

private:
    std::shared_ptr<Environment> globals_;
    std::shared_ptr<Environment> environment_;

    void execute(const StmtPtr& stmt);
    void execute_block(const std::vector<StmtPtr>& statements, std::shared_ptr<Environment> env);
    Value evaluate(const ExprPtr& expr);
};

using RegisterLibraryFn = void(*)(Interpreter&);

} // namespace citril
