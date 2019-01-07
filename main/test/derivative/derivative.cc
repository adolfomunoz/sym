#include <sym.h>
#include <iostream>

template<typename A, typename B>
void test(const sym::expression& f, const sym::symbol& x, const A& a, const B& b) {
	std::cout<<"f = "<<f<<"\tdf/d"<<x<<" = "<<f.derivative(x)<<"\tError = "<<
		sym::integral(sym::pow(f.derivative(x) - sym::numerical_derivative(f,x,sym::expression(1)/1000),2.0f),x,a,b)<<std::endl;
}

int main(int argc, char** argv) {
	sym::symbol x("x");
	test(x,x,sym::minus_infinity,sym::infinity);
	test(x*x,x,sym::minus_infinity,sym::infinity);
	test(x*x*x,x,sym::minus_infinity,sym::infinity);
	test(1/x,x,0.001,sym::infinity);
	test(sym::ln(x),x,0.001,sym::infinity);
}
