#include "citril/interpreter.hpp"
#include "citril/lexer.hpp"
#include "citril/parser.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

int main(int argc, char** argv){
    if(argc<2){ std::cerr<<"Usage: citril <file>\n"; return 1; }
    std::ifstream in(argv[1]); if(!in){ std::cerr<<"Could not open file\n"; return 1; }
    std::stringstream ss; ss<<in.rdbuf();
    try { citril::Lexer lex(ss.str()); auto tokens=lex.scan_tokens(); citril::Parser p(tokens); auto stmts=p.parse(); citril::Interpreter i; i.interpret(stmts); }
    catch(const std::exception& e){ std::cerr<<"Error: "<<e.what()<<"\n"; return 1; }
    return 0;
}
