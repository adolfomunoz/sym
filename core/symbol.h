#pragma once

#include "expression.h"
#include <exception>

namespace sym {

class symbol_eval : public std::runtime_error {
public:
	symbol_eval(const std::string& name) :
		std::runtime_error(std::string("Trying to evaluate symbol ")+name) { }
};

class Symbol : public Expression {
	static unsigned int ids;
	unsigned int id;
	std::string name;
public:
	Symbol(const std::string& name) : id(++ids), name(name) { }

	bool operator==(const Symbol& that) const { return this->id == that.id; }
	bool operator!=(const Symbol& that) const { return this->id != that.id; }

	std::string to_string() const override { return name; }
	float eval() const override { throw symbol_eval(name); }
	
	std::shared_ptr<Expression> subs(const Symbol& symbol, const std::shared_ptr<Expression>& e) const override {
		if (symbol == (*this)) return e; else return std::make_shared<Symbol>(*this);
	}	
};

unsigned int Symbol::ids = 0;


}
