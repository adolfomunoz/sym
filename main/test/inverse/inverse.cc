#include <sym.h>
#include <iostream>
#include <iomanip>



template<typename A, typename B>
void test(const sym::expression& f, const sym::symbol& x, const sym::symbol& y, const A& a, const B& b) {
	std::cout<<y<<" = "<<f<<"\t";
	try {
		sym::expression f_inv = f.inverse(x,y); 
		//vv We use "sym::id(f)" instead of f because we want to avoid the automatic simplifications. The identity function servers as a wrapper.
		float error = std::sqrt(sym::integral(sym::pow(x - f_inv.substitute(y,sym::id(f)),2),x,a,b)/sym::integral(x*x,x,a,b));
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
	test(x*x*x,x,y,sym::minus_infinity,sym::infinity);
	test(1/x,x,y,0.001,sym::infinity);
	test(sym::ln(x),x,y,0.001,sym::infinity);
	test(sym::log(x,2),x,y,0.1,100);
	test(sym::pow(sym::e,x),x,y,sym::minus_infinity, sym::infinity);
	test(sym::sin(x),x,y,sym::minus_infinity, sym::infinity);
	test(sym::cos(x),x,y,sym::minus_infinity, sym::infinity);
	test(sym::asin(x),x,y,-0.999, 0.999);
	test(sym::acos(x),x,y,-0.999, 0.999);
	test(3*sym::sin(sym::ln(x)),x,y,0.001, sym::infinity);
}
