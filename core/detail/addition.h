#pragma once

#include "expression-binary.h"

namespace sym {

namespace detail {
class Addition : public VisitableDerived<Addition,ExpressionBinary> {
public:
	using VisitableDerived<Addition,ExpressionBinary>::VisitableDerived;
	
	std::string to_string() const override { return subexpression_to_string(expression1())+" + "+subexpression_to_string(expression2()); }
	float evaluate() const override { return expression1().evaluate()+expression2().evaluate(); }
	expression substitute(const symbol& s, const expression& e) const override {
		return expression1().substitute(s,e)+expression2().substitute(s,e); 
	}
	expression derivative(const symbol& s) const override {
		return expression1().derivative(s) + expression2().derivative(s);
	}
	int precedence() const override { return 3; }
};

inline expression add_default(const expression& e1, const expression& e2) {
	return expression(std::make_shared<Addition>(e1,e2));
}

//Default add for double dispatch. Must create new "add" versions for specific subtypes of detail::Expression
template<typename E1, typename E2>
expression add(const E1& e1, const E2& e2) {
	return add_default(e1, e2);
}

}
}
