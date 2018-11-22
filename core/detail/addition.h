#pragma once

#include "../expression.h"

namespace sym {

namespace detail {
class Addition : public Expression {
	expression e1_, e2_;
public:
	Addition(const expression& e1, const expression& e2) : e1_(e1), e2_(e2) { }

	std::string to_string() const override { return e1_.to_string()+" + "+e2_.to_string(); }
	float evaluate() const override { return e1_.evaluate()+e2_.evaluate(); }
	expression substitute(const symbol& s, const expression& e) const override {
		return e1_.substitute(s,e)+e2_.substitute(s,e); 
	}
	bool depends_on(const symbol& s) const override {
		return e1_.depends_on(s) || e2_.depends_on(s);
	}
	expression derivative(const symbol& s) const override {
		return e1_.derivative(s) + e2_.derivative(s);
	}
};

}
}
