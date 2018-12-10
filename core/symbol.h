#pragma once

#include "detail/symbol.h"
#include <exception>

namespace sym {

// Wrapper to avoid using shared pointers all the time everywhere and for easy use, and to ensure symbol identification through to its memory address
class symbol : public expression {
	friend class detail::Symbol;
	symbol(const std::shared_ptr<const detail::Symbol>& s) : expression(s) { }
public:
	symbol(const std::string& name) : expression(std::make_shared<detail::Symbol>(name)) { } //This is the only case in which we create the symbol

	//Copy/move constructors and assignments copy/move the pointer, which is what we want.
	//operator== compares pointers, which is what we want.
};

class not_invertible_error : public std::runtime_error {
	expression ex_;
	symbol s_;
public:
	not_invertible_error(const expression& ex, const symbol& s):
		std::runtime_error(ex.to_string()+" is not invertible with respect to "+s.to_string()), ex_(ex), s_(s) { }
	
	const expression& ex() const { return ex_; }
	const symbol& s() const { return s_; }
};


}
