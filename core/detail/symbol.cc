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
}
}
