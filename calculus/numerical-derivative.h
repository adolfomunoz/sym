#pragma once
#include "../core/expression.h"

namespace sym {

inline expression numerical_derivative(const expression& f, const symbol& x, const expression& h) {
//	return (-f.substitute(x,x+2*h) + 8*f.substitute(x,x+h) - 8*f.substitute(x,x-h) + f.substitute(x,x-2*h))/(12*h);
	return (f.substitute(x,x+h) - f.substitute(x,x-h))/(2*h);
//	return (f.substitute(x,x+h) - f)/h;
}

}
