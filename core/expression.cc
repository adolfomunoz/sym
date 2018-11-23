#include "expression.h"
#include "detail/constant.h"

namespace sym {

expression::expression(int i)   : expression::expression(std::make_shared<detail::Constant<int>>(i)) { }
expression::expression(float f) : expression::expression(std::make_shared<detail::Constant<float>>(f)) { }

std::ostream& operator<<(std::ostream& os, const expression& e) {
	os<<e.to_string();
	return os;
}

};
