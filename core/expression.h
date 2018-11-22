#pragma once

#include "detail/expression.h"
#include <iostream>

namespace sym {

namespace detail {	class Symbol;	}

// Wrapper to avoid using shared pointers all the time everywhere and for easy use
class expression {
	std::shared_ptr<const detail::Expression> e_;
private:
	const std::shared_ptr<const detail::Expression>& internal_pointer() const { return e_; }
	friend class detail::Symbol;
protected:
	expression(const std::shared_ptr<const detail::Expression>& e) : e_(e) { }
public:
	expression(int   i);
	expression(float f);

	std::string to_string() const { return e_->to_string(); }
	float evaluate() const { return e_->evaluate(); }
	expression substitute(const symbol& s, const expression& e) const {
		return expression(e_->substitute(s,e));
	}
};

std::ostream& operator<<(std::ostream& os, const expression& e);

}
