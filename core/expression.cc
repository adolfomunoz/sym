#include "expression.h"
#include "detail/constant.h"
#include "detail/addition.h"
#include "detail/product.h"
#include "detail/symbol.h"

namespace sym {

expression::expression(int i)   : expression::expression(std::make_shared<detail::Constant<int>>(i)) { }
expression::expression(float f) : expression::expression(std::make_shared<detail::Constant<float>>(f)) { }

expression expression::operator+(const expression& that) const {
	return apply([] (const auto& e1, const auto& e2) { return detail::add(e1,e2); },*this,that);
}

expression expression::operator*(const expression& that) const {
	return apply([] (const auto& e1, const auto& e2) { return detail::multiply(e1,e2); },*this,that);
}

std::ostream& operator<<(std::ostream& os, const expression& e) {
	os<<e.to_string();
	return os;
}

};
