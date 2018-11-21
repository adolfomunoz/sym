#pragma once

#include <memory>
#include <string>

namespace sym {

class Symbol;

class Expression {
public:
	virtual std::string to_string() const = 0;
	virtual float eval() const = 0;	
	virtual std::shared_ptr<Expression> subs(const Symbol& symbol, const std::shared_ptr<Expression>& e) const = 0;
};

// Wrapper to avoid using shared pointers all the time everywhere and for easy use
class ex {
	std::shared_ptr<Expression> e;
public:
	ex(const std::shared_ptr<Expression>& e) : e(e) { }

	std::string to_string() const { return e->to_string(); }
	float eval() const { return e->eval(); }
	ex subs(const Symbol& symbol, const ex& replacement) const {
		return ex(e->subs(symbol,replacement.e));
	}
};

}
