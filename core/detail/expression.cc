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

std::string Expression::subexpression_to_string(const expression& sub) const {
	if (this->precedence()>=sub.internal_pointer()->precedence()) 
		return std::string("(")+sub.to_string()+")";
	else
		return sub.to_string();
}




}
}
