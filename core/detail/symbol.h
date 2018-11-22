#pragma once

#include "../expression.h"
#include <exception>

namespace sym {

class symbol_evaluation_error : public std::runtime_error {
public:
	symbol_evaluation_error(const std::string& name) :
		std::runtime_error(std::string("Trying to evaluate symbol ")+name) { }
};

namespace detail {
class Symbol : public Expression, public std::enable_shared_from_this<Symbol> {
	std::string name;
public:
	Symbol(const std::string& name) : name(name) { }

	std::string to_string() const override { return name; }
	float evaluate() const override { throw symbol_evaluation_error(name); }
	
	expression substitute(const symbol& s, const expression& e) const override;
};

}


}
