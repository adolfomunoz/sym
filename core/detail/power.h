#pragma once

#include "expression-binary.h"
#include "logarithm.h"
#include <cmath>

namespace sym {
namespace detail {

class Power : public VisitableDerived<Power,ExpressionBinary> {
public:
	using VisitableDerived<Power,ExpressionBinary>::VisitableDerived;

	std::string to_string() const override { return subexpression_to_string(expression1())+"^{"+expression2().to_string()+"}"; }
	expression base() const { return expression1(); }
	expression exponent() const { return expression2(); }

	float evaluate() const override { 
		return std::pow(base().evaluate(), exponent().evaluate());
	}

	expression substitute(const expression& s, const expression& e) const override {
		return sym::pow(expression1().substitute(s,e), expression2().substitute(s,e));
	}

	expression derivative(const symbol& s) const override {
		return exponent()*sym::pow(base(),exponent()-1)*base().derivative(s) + sym::pow(base(),exponent())*sym::ln(base())*exponent().derivative(s);
	}


	expression inverse(const symbol& in, const expression& out) const override {
		if (base().depends_on(in)) {
			if (exponent().depends_on(in)) throw not_invertible_error(*this,in);
			else {
				return base().inverse(in, sym::pow(out, -exponent()));
			}
		} else if (exponent().depends_on(in)) {
			return exponent().inverse(in, sym::log(base(), out));
		} else throw not_invertible_error(*this, in);
	}
	


};


inline expression pow_default(const expression& base, const expression& exponent) {
	return expression(std::make_shared<Power>(base,exponent));
}

template<typename E1, typename E2>
expression pow(const E1& base, const E2& exponent) {
	return pow_default(base,exponent);
}

template<typename E>
expression log(const E& base, const Power& p) {
	if (base == p.base()) return p.exponent();
	else return log_default(base,p);
}

template<typename E>
expression pow(const E& base, const Logarithm& l) {
	if (base == l.base()) return l.number();
	else return pow_default(base,l);
}

template<typename E>
expression pow(const Logarithm& l, const E& e) {
	return e*l;
}

template<typename E>
expression pow(const Power& p, const E& exponent) {
	return pow(p.base(),exponent*p.exponent());
}



template<typename E>
expression multiply(const E& e1, const E& e2) {
	if (e1 == e2) return pow(e1, 2);
	else return multiply_default(e1,e2);
}

template<typename E>
expression multiply(const E& e, const Power& p) {
	if (p.base() == e) return pow(e, p.exponent() + 1);
	else return multiply_default(e,p);
}

template<typename E>
expression multiply(const Power& p, const E& e) {
	if (p.base() == e) return pow(e, p.exponent() + 1);
	else return multiply_default(e,p);
}

inline expression multiply(const Power& p1, const Power& p2) {
	if (p1.base() == p2.base()) return pow(p1.base(), p1.exponent() + p2.exponent());
	else return multiply_default(p1,p2);
}

}
}
