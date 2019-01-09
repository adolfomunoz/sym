#include <sym.h>
#include <iostream>
#include <iomanip>



template<typename A, typename B>
void test(const sym::expression& f, const sym::symbol& x, const sym::symbol& y, const A& a, const B& b) {
	std::cout<<y<<" = "<<f<<"\t";
	try {
		sym::expression f_inv = f.inverse(x,y);
		float error = std::sqrt(sym::integral(sym::pow(x - f_inv.substitute(y,f),2),x,a,b)/sym::integral(x*x,x,a,b));
		std::cout<<x<<" = "<<f_inv<<"\tError = "<<std::fixed<<std::setprecision(2)<<100.0*error<<"\%"<<std::endl;
	} catch(const sym::not_invertible_error& e) {
		std::cout<<e.what()<<std::endl;
	}	
}

int main(int argc, char** argv) {
	sym::symbol x("x");
	sym::symbol y("y");
	test(x,x,y,sym::minus_infinity,sym::infinity);
	test(x*x,x,y,sym::minus_infinity,sym::infinity);
}
