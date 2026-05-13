#include "citril/lexer.hpp"
#include "citril/error.hpp"
#include <cctype>
#include <unordered_map>
namespace citril {
Lexer::Lexer(std::string source) : source_(std::move(source)) {}
std::string token_type_name(TokenType type) { return std::to_string(static_cast<int>(type)); }
std::vector<Token> Lexer::scan_tokens(){ while(!is_at_end()){ start_=current_; token_column_=column_; scan_token(); } tokens_.push_back({TokenType::EndOfFile,"",line_,column_}); return tokens_; }
bool Lexer::is_at_end() const { return current_>=source_.size(); }
char Lexer::advance(){ column_++; return source_[current_++]; }
bool Lexer::match(char expected){ if(is_at_end()||source_[current_]!=expected) return false; current_++; column_++; return true; }
char Lexer::peek() const { return is_at_end()?'\0':source_[current_]; }
char Lexer::peek_next() const { return current_+1>=source_.size()?'\0':source_[current_+1]; }
void Lexer::add_token(TokenType type){ tokens_.push_back({type,source_.substr(start_,current_-start_),line_,token_column_}); }
void Lexer::scan_token(){ char c=advance(); switch(c){ case '(':add_token(TokenType::LeftParen);break; case ')':add_token(TokenType::RightParen);break; case '{':add_token(TokenType::LeftBrace);break; case '}':add_token(TokenType::RightBrace);break; case ',':add_token(TokenType::Comma);break; case '.':add_token(TokenType::Dot);break; case '-':add_token(TokenType::Minus);break; case '+':add_token(TokenType::Plus);break; case ';':add_token(TokenType::Semicolon);break; case '*':add_token(TokenType::Star);break; case '%':add_token(TokenType::Percent);break; case '!':add_token(match('=')?TokenType::BangEqual:TokenType::Bang);break; case '=':add_token(match('=')?TokenType::EqualEqual:TokenType::Equal);break; case '<':add_token(match('=')?TokenType::LessEqual:TokenType::Less);break; case '>':add_token(match('=')?TokenType::GreaterEqual:TokenType::Greater);break; case '/': if(match('/')) while(peek()!='\n'&&!is_at_end()) advance(); else add_token(TokenType::Slash); break; case '#': while(peek()!='\n'&&!is_at_end()) advance(); break; case ' ':case '\r':case '\t': break; case '\n': line_++; column_=1; break; case '"':scan_string(); break; default: if(std::isdigit(c)) number(); else if(std::isalpha(c)||c=='_') identifier(); else throw LexError("Unexpected character"); }}
void Lexer::scan_string(){ while(peek()!='"'&&!is_at_end()){ if(peek()=='\n'){line_++; column_=1;} advance(); } if(is_at_end()) throw LexError("Unterminated string"); advance(); add_token(TokenType::String); }
void Lexer::number(){ while(std::isdigit(peek())) advance(); if(peek()=='.'&&std::isdigit(peek_next())){ advance(); while(std::isdigit(peek())) advance(); } add_token(TokenType::Number); }
void Lexer::identifier(){ while(std::isalnum(peek())||peek()=='_') advance(); std::string text=source_.substr(start_,current_-start_); static const std::unordered_map<std::string,TokenType> k{{"and",TokenType::And},{"else",TokenType::Else},{"false",TokenType::False},{"fn",TokenType::Fn},{"if",TokenType::If},{"let",TokenType::Let},{"localvar",TokenType::LocalVar},{"nil",TokenType::Nil},{"or",TokenType::Or},{"return",TokenType::Return},{"true",TokenType::True},{"var",TokenType::Var},{"while",TokenType::While}}; auto it=k.find(text); tokens_.push_back({it==k.end()?TokenType::Identifier:it->second,text,line_,token_column_}); }
void Lexer::identifier(){ while(std::isalnum(peek())||peek()=='_') advance(); std::string text=source_.substr(start_,current_-start_); static const std::unordered_map<std::string,TokenType> k{{"and",TokenType::And},{"else",TokenType::Else},{"false",TokenType::False},{"fn",TokenType::Fn},{"if",TokenType::If},{"let",TokenType::Let},{"nil",TokenType::Nil},{"or",TokenType::Or},{"return",TokenType::Return},{"true",TokenType::True},{"while",TokenType::While}}; auto it=k.find(text); tokens_.push_back({it==k.end()?TokenType::Identifier:it->second,text,line_,token_column_}); }
}
