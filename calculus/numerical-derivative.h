#include "../core/expression.h"

namespace sym {

expression numerical_derivative(const expression& f, const symbol& x, const expression& h) {
//	return (-f.substitute(x,x+2*h) + 8*f.substitute(x,x+h) - 8*f.substitute(x,x-h) + f.substitute(x,x-2*h))/(12*h);
//	return (f.substitute(x,x+h) - f.substitute(x,x-h))/(2*h);
//	These two more accurate above do not seem to work. That might be a sign that something is off.
	return (f.substitute(x,x+h) - f)/h;
}

}
