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
	
	expression add_to(const expression& that) const override{
		if (value() == Num(0)) return that;
		else return that.apply(overloaded {
			[this] (const auto& ex) {
				return addition(ex,*this);
			},
			[this] (const Constant<Num>& c) {
				return expression(this->value()+c.value());
			}
		});
	}

	expression multiply_by(const expression& that) const override{
		if (value() == Num(0)) return expression(0);
		else if (value() == Num(1)) return that;
		else return that.apply(overloaded {
			[this] (const auto& ex) {
				return product(*this,ex);
			},
			[this] (const Constant<Num>& c) {
				return expression(this->value()*c.value());
			}
		});
	}

};

}
}
