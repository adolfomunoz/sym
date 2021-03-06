#pragma once

#include "../expression.h"

namespace sym {

namespace detail {
class ExpressionBinary : public Expression {
	expression e1_, e2_;
public:
	ExpressionBinary(const expression& e1, const expression& e2) : e1_(e1), e2_(e2) { }
	const expression& expression1() const { return e1_; }
	const expression& expression2() const { return e2_; }

	bool depends_on(const symbol& s) const override {
		return expression1().depends_on(s) || expression2().depends_on(s);
	}

	template<typename F>
	auto apply(const F& f) const {
		return apply(f,expression1(),expression2());
	}

	//This is a problem, we have to improve it (it only works when both types are the same)
	bool operator==(const ExpressionBinary& that) const { 
		return (this->expression1()==that.expression1()) && (this->expression2()==that.expression2());
       	}
};

}
}
