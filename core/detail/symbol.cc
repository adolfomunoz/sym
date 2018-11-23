#include "symbol.h"
#include "../symbol.h"

namespace sym {

namespace detail {
bool Symbol::depends_on(const symbol& s) const {
	return (s.internal_pointer() == this->shared_from_this());
}
}
}
