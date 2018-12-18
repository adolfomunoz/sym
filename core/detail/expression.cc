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
	if (this->precedence()>sub.internal_pointer()->precedence()) 
		return std::string("\\left(")+sub.to_string()+"\\right)";
	else
		return sub.to_string();
}


expression Expression::inverse(const symbol& in, const expression& out) const { throw not_invertible_error(this->to_expression(), in); }

expression Expression::substitute(const expression& source, const expression& replacement) const {
	//By default it does nothing (returns itself)
	return to_expression();
}


}
}
