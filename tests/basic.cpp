#include "citril/interpreter.hpp"
#include "citril/lexer.hpp"
#include "citril/parser.hpp"
int main(){
    const std::string src = "let x = 0; while (x < 3) { x = x + 1; } if (x == 3) { print(\"ok\"); }";
    citril::Lexer lex(src); auto t=lex.scan_tokens(); citril::Parser p(t); auto s=p.parse(); citril::Interpreter i; i.interpret(s);
    return 0;
}
