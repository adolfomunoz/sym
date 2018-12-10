#include "symbol.h"
#include "../symbol.h"

namespace sym {

namespace detail {

symbol Symbol::to_symbol() const {
	return symbol(std::static_pointer_cast<const Symbol>(this->shared_from_this()));
}

Symbol::operator symbol() const {
	return this->to_symbol();
}

bool Symbol::depends_on(const symbol& s) const {
	return (s.internal_pointer() == this->shared_from_this());
}

expression Symbol::inverse(const symbol& in, const expression& out) const {
	if (depends_on(in)) return out; else throw not_invertible_error(to_expression(), in);
}

}
}
