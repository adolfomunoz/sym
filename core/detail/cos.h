#pragma once

#include "expression-unary.h"
#include <cmath>

namespace sym {
namespace detail {

class Cos : public VisitableDerived<Cos,ExpressionUnary> {
public:
	using VisitableDerived<Cos,ExpressionUnary>::VisitableDerived;

	std::string function_name() const override { return std::string("cos"); }
	float function_evaluate(float f) const override {  return std::cos(f); 	}
	expression function(const expression& e) const override { return cos(e); }
	expression function_derivative(const expression& e) const override { return -sin(e); }
	expression function_inverse(const expression& e) const override { return acos(e); }
};


inline expression cos_default(const expression& e) {
	return expression(std::make_shared<Cos>(e));
}

template<typename E>
expression cos(const E& e) {
	return cos_default(e);
}

inline expression acos(const Cos& e) {
	return e.subexpression();
}

}
}
