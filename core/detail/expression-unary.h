#pragma once

#include "../expression.h"

namespace sym {

namespace detail {
class ExpressionUnary : public Expression {
	expression e_
public:
	ExpressionUnary(const expression& e) : e_(e) { }
	const expression& subexpression() const { return e_; }

	bool depends_on(const symbol& s) const override {
		return subexpression().depends_on(s);
	}

	template<typename F>
	auto apply(const F& f) const {
		return apply(f,subexpression(),expression2());
	}

	//This is a problem, we have to improve it (it only works when both types are the same)
	bool operator==(const ExpressionUnary& that) const { 
		return (this->subexpression()==that.subexpression());
       	}

	virtual std::string function_name() const = 0;
};

}
}
