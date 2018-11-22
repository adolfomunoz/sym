#pragma once

#include "detail/symbol.h"
#include <exception>

namespace sym {

// Wrapper to avoid using shared pointers all the time everywhere and for easy use, and to ensure symbol identification through to its memory address
class symbol : public expression {
public:
	symbol(const std::string& name) : expression(std::make_shared<detail::Symbol>(name)) { } //This is the only case in which we create the symbol

	//Copy/move constructors and assignments copy/move the pointer, which is what we want.
	//operator== compares pointers, which is what we want.
};

}
