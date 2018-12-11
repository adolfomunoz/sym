#pragma once

#include "expression-binary.h"
#include "product.h"
#include <cmath>

namespace sym {
namespace detail {

class Logarithm : public VisitableDerived<Logarithm,ExpressionBinary> {
public:
	using VisitableDerived<Logarithm,ExpressionBinary>::VisitableDerived;

	expression base() const { return expression1(); }
	expression number() const { return expression2(); }
	std::string to_string() const override { return std::string("log_{")+base().to_string()+"} \\left("+number().to_string()+"\\right)"; }

	float evaluate() const override { 
		return std::log(number().evaluate())/std::log(base().evaluate());
	}

	expression substitute(const symbol& s, const expression& e) const override {
		return sym::log(expression1().substitute(s,e), expression2().substitute(s,e));
	}

	expression derivative(const symbol& s) const override {
		//We apply the quotient rule over the division of natural logarithms
		return number().derivative(s)/(number()*sym::ln(base())) - sym::ln(number())*base().derivative(s)/(base()*sym::pow(sym::ln(base()),2));
	}

	expression inverse(const symbol& in, const expression& out) const override {
		if (base().depends_on(in)) {
			if (number().depends_on(in)) throw not_invertible_error(*this,in);
			else {
				return base().inverse(in, sym::pow(out, number()));
			}
		} else if (number().depends_on(in)) {
			return number().inverse(in, sym::pow(base(), out));
		} else throw not_invertible_error(*this, in);
	}

};


inline expression log_default(const expression& base, const expression& number) {
	if (base == number) return expression(1);
	else return expression(std::make_shared<Logarithm>(base,number));
}


template<typename E1, typename E2>
expression log(const E1& base, const E2& number) {
	return log_default(base,number);
}

template<typename E>
expression log(const E& base, const Product& number) {
	expression sol = 0;
	for (expression e : number.expressions()) sol += log(base,e);
	return sol;
}




}
}
