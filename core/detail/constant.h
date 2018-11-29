#pragma once

#include "../expression.h"
#include "../symbol.h"
#include "addition.h"
#include "product.h"

namespace sym {

namespace detail {
template<typename Num>
class Constant : public VisitableDerived<Constant<Num>,Expression> {
	Num value_;
public:
	Constant(const Num& value) : value_(value) { }
	const Num& value() const { return value_; }

	//I thought this would be defined by defaut but it seems it is not
	bool operator==(const Constant<Num>& that) const { return this->value() == that.value(); }

	std::string to_string() const override { return std::to_string(value()); }
	float evaluate() const override { return float(value()); }
	expression substitute(const symbol& s, const expression& e) const override {
		return expression(value()); 
	}
	expression derivative(const symbol& s) const override {
		return expression(0);
	}
	bool depends_on(const symbol& s) const override {
		return false;
	}
};


template<typename Num>
expression add(const Constant<Num>& e1, const Constant<Num>& e2) {
	return expression(e1.value()+e2.value());
}

template<typename Num1, typename Num2>
expression add(const Constant<Num1>& e1, const Constant<Num2>& e2) {
	if (e1.value() == Num1(0)) return e2;
	else if (e2.value() == Num2(0)) return e1;
	else return multiply_default(e1,e2);
}


template<typename Ex, typename Num, typename = std::enable_if_t< !std::is_base_of_v<Addition,Ex> && !std::is_base_of_v<Product,Ex> >>
expression add(const Constant<Num>& c, const Ex& e) {
	if (c.value() == Num(0)) return e;
	else return add_default(e,c);
}

template<typename Ex, typename Num, typename = std::enable_if_t< !std::is_base_of_v<Addition,Ex> && !std::is_base_of_v<Product,Ex> >>
expression add(const Ex& e, const Constant<Num>& c) {
	if (c.value() == Num(0)) return e;
	else return add_default(e,c);
}

template<typename Num>
expression multiply(const Constant<Num>& e1, const Constant<Num>& e2) {
	return expression(e1.value()*e2.value());
}

template<typename Num1, typename Num2>
expression multiply(const Constant<Num1>& e1, const Constant<Num2>& e2) {
	if ( (e1.value() == Num1(0)) || (e2.value() == Num2(0)) ) return expression(0);
	else if (e1.value() == Num1(1)) return e2;
	else if (e2.value() == Num2(1)) return e1;
	else return multiply_default(e1,e2);
}


template<typename Ex, typename Num, typename = std::enable_if_t<!std::is_base_of_v<Product,Ex>>>
expression multiply(const Constant<Num>& c, const Ex& e) {
	if (c.value() == Num(0)) return expression(0);
	else if (c.value() == Num(1)) return e;
	else return multiply_default(c,e);
}

template<typename Ex, typename Num, typename = std::enable_if_t<!std::is_base_of_v<Product,Ex>>>
expression multiply(const Ex& e, const Constant<Num>& c) {
	if (c.value() == Num(0)) return expression(0);
	else if (c.value() == Num(1)) return e;
	else return multiply_default(c,e);
}





}
}
