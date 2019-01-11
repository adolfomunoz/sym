#pragma once

#include "expression-unary.h"
#include <cmath>

namespace sym {
namespace detail {

//Identity function. Just helps when avoiding automatic simplifications and stuff. It is 
//included as part of the visitable hierarchy as a helper/wrapper function.
class Id : public VisitableDerived<Id,ExpressionUnary> {
public:
	using VisitableDerived<Id,ExpressionUnary>::VisitableDerived;

	std::string function_name() const override { return std::string("id"); }
	float function_evaluate(float f) const override {  return f; 	}
	expression function(const expression& e) const override { return e; }
	expression function_derivative(const expression& e) const override { return e; }
	expression function_inverse(const expression& e) const override { return e; }
};

}


}
