#include "../expression.h"
#include "addition.h"
#include "product.h"
#include "constant.h"
#include "symbol.h"

namespace sym {
namespace detail {

expression Expression::to_expression() const {
	return expression(this->shared_from_this());
}

Expression::operator expression() const {
	return this->to_expression();
}

expression Expression::add_to(const expression& that) const {
	return that.apply(overloaded {
		[this] (const auto& ex) {
			return addition(*this,ex);
		},
		[this] (const Constant<int>& c) {
			if (c.value() == 0) return this->to_expression();
			else return addition(*this, c);
		},
		[this] (const Constant<float>& c) {
			if (c.value() == 0.0f) return this->to_expression();
			else return addition(*this, c);
		}
	});
}

expression Expression::multiply_by(const expression& that) const {
	return that.apply(overloaded {
		[this] (const auto& ex) {
			return product(*this, ex);
		},
		[this] (const Constant<int>& c) {
			if (c.value() == 0) return expression(0);
			else if (c.value() == 1) return this->to_expression();
			else return product(c, *this);
		},
		[this] (const Constant<float>& c) {
			if (c.value() == 0.0f) return expression(0);
			else if (c.value() == 1.0f) return this->to_expression();
			else return product(c, *this);
		}
	});
}



}
}
