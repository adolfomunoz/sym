#include "symbol.h"
#include "../symbol.h"

namespace sym {

namespace detail {
expression Symbol::substitute(const symbol& s, const expression& e) const {
	if (s.internal_pointer() == shared_from_this()) return e; else return expression(this->shared_from_this());
}
}
}
