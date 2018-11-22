#include "expression.h"
#include "detail/constant.h"
#include "detail/addition.h"

namespace sym {

expression::expression(int i)   : expression::expression(std::make_shared<detail::Constant<int>>(i)) { }
expression::expression(float f) : expression::expression(std::make_shared<detail::Constant<float>>(f)) { }

expression expression::operator+(const expression& that) const {
	return expression(std::make_shared<detail::Addition>(*this, that));
}

std::ostream& operator<<(std::ostream& os, const expression& e) {
	os<<e.to_string();
	return os;
}

};
