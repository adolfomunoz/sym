#include <sym.h>
#include <iostream>

void show(const sym::symbol& x, const sym::symbol& y, const sym::expression& ex) {
	std::cout<<y<<" = "<<ex<<std::endl;
	try {
		std::cout<<x<<" = "<<ex.inverse(x,y)<<std::endl;
	} catch(const sym::not_invertible_error& e) {
		std::cout<<e.what()<<std::endl;
	}
	std::cout<<std::endl;	
}

int main(int argc, char** argv) {
	sym::symbol x("x");
	sym::symbol y("y");
	sym::symbol a("a");
	show(x,y,x);
	show(x,y,x-1);
	show(x,y,x+a);
	show(x,y,3*x);
	show(x,y,a*x);
	show(x,y,2*x+3);
	show(x,y,a*x+3);
	show(x,y,2*x+a);
	show(x,y,sym::pow(x,a));
	show(x,y,sym::pow(sym::e,x));
	show(x,y,sym::log(x,a));
	show(x,y,sym::log(sym::e,x));
	show(x,y,sym::cos(x));
	show(x,y,sym::sin(a*x));
	show(x,y,sym::pow(sym::sin(x),a));
}
