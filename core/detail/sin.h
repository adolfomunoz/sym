#pragma once

#include "expression-unary.h"
#include <cmath>

namespace sym {
namespace detail {

class Sin : public VisitableDerived<Sin,ExpressionUnary> {
public:
	using VisitableDerived<Sin,ExpressionUnary>::VisitableDerived;

	std::string function_name() const override { return std::string("sin"); }
	float function_evaluate(float f) const override {  return std::sin(f); 	}
	expression function(const expression& e) const override { return sin(e); }
	expression function_derivative(const expression& e) const override { return cos(e); }
	expression function_inverse(const expression& e) const override { return asin(e); }
};


inline expression sin_default(const expression& e) {
	return expression(std::make_shared<Sin>(e));
}

template<typename E>
expression sin(const E& e) {
	return sin_default(e);
}

inline expression asin(const Sin& e) {
	return e.subexpression();
}

}
}
