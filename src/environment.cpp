#include "citril/environment.hpp"
namespace citril {
Environment::Environment(std::shared_ptr<Environment> enclosing) : enclosing_(std::move(enclosing)) {}
void Environment::define(const std::string& name, Value value){ values_[name]=std::move(value);} 
Value Environment::get(const Token& name) const { auto it=values_.find(name.lexeme); if(it!=values_.end()) return it->second; if(enclosing_) return enclosing_->get(name); throw RuntimeError("Undefined variable: "+name.lexeme); }
void Environment::assign(const Token& name, Value value){ auto it=values_.find(name.lexeme); if(it!=values_.end()){ it->second=std::move(value); return;} if(enclosing_){ enclosing_->assign(name,std::move(value)); return;} throw RuntimeError("Undefined variable: "+name.lexeme); }
}
