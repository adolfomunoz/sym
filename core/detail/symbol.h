#pragma once

#include "../expression.h"
#include <exception>

namespace sym {

class symbol_evaluation_error : public std::runtime_error {
	std::string name_;
public:
	symbol_evaluation_error(const std::string& name) :
		std::runtime_error(std::string("Trying to evaluate symbol ")+name), name_(name) { }
	const std::string& name() const { return name_; }
};

class symbol;

namespace detail {
class Symbol : public VisitableDerived<Symbol,Expression> {
	std::string name;
public:
	Symbol(const std::string& name) : name(name) { }

	std::string to_string() const override { return name; }
	float evaluate() const override { throw symbol_evaluation_error(name); }
	
	bool depends_on(const symbol& s) const override;
	expression substitute(const symbol& s, const expression& e) const override {
		if (depends_on(s)) return e; else return *this;
	}
	expression derivative(const symbol& s) const override {
		if (depends_on(s)) return expression(1); else return expression(0);
	}
	
	bool operator==(const Symbol& s) const { return s.shared_from_this() == this->shared_from_this(); }

	symbol to_symbol() const;
	operator symbol() const;
};

}


}
