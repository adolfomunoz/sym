#pragma once

#include "expression-unary.h"
#include <cmath>

namespace sym {
namespace detail {

class Acos : public VisitableDerived<Acos,ExpressionUnary> {
public:
	using VisitableDerived<Acos,ExpressionUnary>::VisitableDerived;

	std::string function_name() const override { return std::string("arccos"); }
	float function_evaluate(float f) const override {  return std::acos(f); 	}
	expression function(const expression& e) const override { return acos(e); }
	expression function_derivative(const expression& e) const override { return -pow(1 - e*e,-expression(1)/2); }
	expression function_inverse(const expression& e) const override { return cos(e); }
};


inline expression acos_default(const expression& e) {
	return expression(std::make_shared<Acos>(e));
}

template<typename E>
expression acos(const E& e) {
	return acos_default(e);
}

inline expression cos(const Acos& e) {
	return e.subexpression();
}



}
}
