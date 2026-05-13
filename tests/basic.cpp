#include "citril/interpreter.hpp"
#include "citril/lexer.hpp"
#include "citril/parser.hpp"
int main(){
    const std::string src = "var x = 0 while (x < 3) { localvar t = x x = x + 1 } if (x == 3) { ok() }";
    citril::Lexer lex(src); auto t=lex.scan_tokens(); citril::Parser p(t); auto s=p.parse(); citril::Interpreter i;
    i.register_native("ok", 0, [](const std::vector<citril::Value>&){ return citril::Nil{}; });
    i.interpret(s);
    return 0;
}
