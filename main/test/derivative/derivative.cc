#include <sym.h>
#include <iostream>

template<typename A, typename B>
void test(const sym::expression& f, const sym::symbol& x, const A& a, const B& b) {
	sym::expression d1 = f.derivative(x);
	sym::expression d2 = sym::numerical_derivative(f,x,sym::expression(1)/1000);
	for (float t = 0.1f; t<1.0f; t+=0.1f) 
		std::cout<<t<<"\t"<<d1.substitute(x,t)<<"\t"<<d2.substitute(x,t)<<std::endl;


	std::cout<<"f = "<<f<<"\tdf/d"<<x<<" = "<<f.derivative(x)<<" "<<sym::numerical_derivative(f,x,sym::expression(1)/1000)<<"\tError = "<<
		sym::integral(sym::pow(f.derivative(x) - sym::numerical_derivative(f,x,sym::expression(1)/1000),2.0f),x,a,b)<<std::endl;
}

int main(int argc, char** argv) {
	sym::symbol x("x");
	test(x,x,sym::minus_infinity,sym::infinity);
	test(x*x,x,sym::minus_infinity,sym::infinity);
	test(x*x*x,x,sym::minus_infinity,sym::infinity);
	test(1/x,x,0.001,sym::infinity);
	test(sym::ln(x),x,0.001,sym::infinity);
	test(sym::log(x,10),x,0.1,0.9);
	test(sym::pow(sym::e,x),x,sym::minus_infinity, sym::infinity);
	test(sym::sin(x),x,sym::minus_infinity, sym::infinity);
	test(sym::cos(x),x,sym::minus_infinity, sym::infinity);
	test(sym::asin(x),x,-0.999, 0.999);
	test(sym::acos(x),x,-0.999, 0.999);
	test(3*sym::sin(sym::ln(x)),x,0.001, sym::infinity);
	
}
