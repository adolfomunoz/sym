#pragma once

#include "expression-binary.h"

namespace sym {

namespace detail {
class Product : public VisitableDerived<Product,ExpressionBinary> {
public:
	using VisitableDerived<Product,ExpressionBinary>::VisitableDerived;

	std::string to_string() const override { return subexpression_to_string(expression1())+subexpression_to_string(expression2()); }
	float evaluate() const override { return expression1().evaluate()*expression2().evaluate(); }
	expression substitute(const symbol& s, const expression& e) const override {
		return expression1().substitute(s,e)*expression2().substitute(s,e); 
	}
	expression derivative(const symbol& s) const override {
		return expression1()*expression2().derivative(s) + expression1().derivative(s)*expression2();
	}
	int precedence() const override { return 4; }

	
};

inline expression multiply_default(const expression& e1, const expression& e2) {
	return expression(std::make_shared<Product>(e1,e2));
}

template<typename E1, typename E2>
expression multiply(const E1& e1, const E2& e2) {
	return multiply_default(e1,e2);
}

}
}
