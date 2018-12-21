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
				return base().inverse(in, sym::pow(out, 1/exponent()));
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
	if (base.to_expression() == p.base()) return p.exponent();
	else return log_default(base,p);
}

template<typename E>
expression pow(const E& base, const Logarithm& l) {
	if (base.to_expression() == l.base()) return l.number();
	else return pow_default(base,l);
}

template<typename Num>
expression pow(const Constant<Num>& c, const Logarithm& l) {
	if (c.value() == Num(0)) return expression(0);
	else if (c.value() == Num(1)) return expression(1);
	else if (c.to_expression() == l.base()) return l.number();
	else return pow_default(c,l);
}


template<typename E>
expression pow(const Logarithm& l, const E& e) {
	return expression(e)*l;
}

inline expression pow(const Logarithm& l, const Logarithm& l2) {
	if (l.to_expression() == l2.base()) return l2.number();
	return l2.to_expression()*l;
}


template<typename E>
expression pow(const Power& p, const E& exponent) {
	return pow(p.base(),exponent.to_expression()*p.exponent());
}

inline expression pow(const Power& base, const Logarithm& l) {
	if (base.to_expression() == l.base()) return l.number();
	else return pow(base.base(),l.to_expression()*base.exponent());
}


template<typename E>
expression multiply(const E& e1, const E& e2) {
	if (e1 == e2) return pow(e1, 2);
	else return multiply_default(e1,e2);
}

template<typename E, typename = std::enable_if_t<!std::is_base_of_v<Product,E>>>
expression multiply(const E& e, const Power& p) {
	if (p.base() == e) return pow(e, p.exponent() + 1);
	else return multiply_default(e,p);
}

template<typename Num>
expression multiply(const Constant<Num>& e, const Power& p) {
	if (e.value() == Num(0)) return expression(0);
	else if (e.value() == Num(1)) return p;
	else if (p.base() == e.to_expression()) return pow(e, p.exponent() + 1);
	else return multiply_default(e,p);
}


template<typename E, typename = std::enable_if_t<!std::is_base_of_v<Product,E>>>
expression multiply(const Power& p, const E& e) {
	if (p.base() == e) return pow(e, p.exponent() + 1);
	else return multiply_default(e,p);
}

template<typename Num>
expression multiply(const Power& p, const Constant<Num>& e) {
	if (e.value() == Num(0)) return expression(0);
	else if (e.value() == Num(1)) return p;
	else if (p.base() == e.to_expression()) return pow(e, p.exponent() + 1);
	else return multiply_default(e,p);
}


inline expression multiply(const Power& p1, const Power& p2) {
	if (p1.base() == p2.base()) return pow(p1.base(), p1.exponent() + p2.exponent());
	else return multiply_default(p1,p2);
}

}
}
