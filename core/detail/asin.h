#pragma once

#include "expression-unary.h"
#include <cmath>

namespace sym {
namespace detail {

class Asin : public VisitableDerived<Asin,ExpressionUnary> {
public:
	using VisitableDerived<Asin,ExpressionUnary>::VisitableDerived;

	std::string function_name() const override { return std::string("arcsin"); }
	float function_evaluate(float f) const override {  return std::asin(f); 	}
	expression function(const expression& e) const override { return asin(e); }
	expression function_derivative(const expression& e) const override { return pow(1 - e*e,-expression(1)/2); }
	expression function_inverse(const expression& e) const override { return sin(e); }
};


inline expression asin_default(const expression& e) {
	return expression(std::make_shared<Asin>(e));
}

template<typename E>
expression asin(const E& e) {
	return asin_default(e);
}

inline expression sin(const Asin& e) {
	return e.subexpression();
}

}
}
