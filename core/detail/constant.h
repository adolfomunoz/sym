#pragma once

#include "../expression.h"
#include "../symbol.h"

namespace sym {

namespace detail {
template<typename Num>
class Constant : public Expression {
	Num c;
public:
	Constant(const Num& c) : c(c) { }

	std::string to_string() const override { return std::to_string(c); }
	float evaluate() const override { return float(c); }
	expression substitute(const symbol& s, const expression& e) const override {
		return expression(c); 
	}
	bool depends_on(const symbol& s) const override {
		return false;
	}
};

}
}
