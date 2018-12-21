#pragma once

#include "../expression.h"

namespace sym {

namespace detail {
class ExpressionUnary : public Expression {
	expression e_;
public:
	ExpressionUnary(const expression& e) : e_(e) { }
	const expression& subexpression() const { return e_; }

	virtual float function_evaluate(float f) const = 0;
	float evaluate() const override { return function_evaluate(subexpression().evaluate()); }

	virtual expression function(const expression& e) const = 0;
	expression substitute(const expression& s, const expression& e) const override {
		return function(subexpression().substitute(s,e));
	}

	virtual expression function_derivative(const expression& e) const = 0;
	expression derivative(const symbol& s) const override {
		return function_derivative(subexpression())*subexpression().derivative(s);
	}

	virtual expression function_inverse(const expression& e) const = 0;
	expression inverse(const symbol& in, const expression& out) const override {
		return subexpression().inverse(in, function_inverse(out));
	}

	bool depends_on(const symbol& s) const override {
		return subexpression().depends_on(s);
	}

	template<typename F>
	auto apply(const F& f) const {
		return apply(f,subexpression());
	}

	//This is a problem, we have to improve it (it only works when both types are the same)
	bool operator==(const ExpressionUnary& that) const { 
		return (this->subexpression()==that.subexpression());
       	}

	virtual std::string function_name() const = 0;
	std::string to_string() const override { return function_name()+"\\left("+subexpression().to_string()+"\\right)"; }
};

}
}
