#include <sym.h>
#include <iostream>
#include <iomanip>

template<typename A, typename B>
void test(const sym::expression& f, const sym::symbol& x, const A& a, const B& b) {
	float error = std::sqrt(sym::integral(sym::pow(f.derivative(x) - sym::numerical_derivative(f,x,sym::expression(1)/1000),2),x,a,b)/sym::integral(sym::pow(f.derivative(x),2),x,a,b));
	std::cout<<"f = "<<f<<"\tdf/d"<<x<<" = "<<f.derivative(x)<<"\tError = "<<std::fixed<<std::setprecision(2)<<100.0*error<<"\%"<<std::endl;
}

int main(int argc, char** argv) {
	sym::symbol x("x");
	test(x,x,sym::minus_infinity,sym::infinity);
	test(x*x,x,sym::minus_infinity,sym::infinity);
	test(x*x*x,x,sym::minus_infinity,sym::infinity);
	test(1/x,x,0.001,sym::infinity);
	test(sym::ln(x),x,0.001,sym::infinity);
	test(sym::log(x,10),x,0.1,sym::infinity);
	test(sym::pow(sym::e,x),x,sym::minus_infinity, sym::infinity);
	test(sym::sin(x),x,sym::minus_infinity, sym::infinity);
	test(sym::cos(x),x,sym::minus_infinity, sym::infinity);
	test(sym::asin(x),x,-0.999, 0.999);
	test(sym::acos(x),x,-0.999, 0.999);
	test(3*sym::sin(sym::ln(x)),x,0.001, sym::infinity);
	
}
