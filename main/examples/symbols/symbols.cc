#include <sym.h>
#include <iostream>



int main(int argc, char** argv) {
	sym::symbol x("x");
	sym::symbol y("y");
	sym::expression e1(16);
	sym::expression e2(x);

	std::cout<<"Expressions   : "<<e1<<" | "<<e2<<std::endl;
	std::cout<<"Substitute x  : "<<e1.substitute(x,e1)<<" | "<<e2.substitute(x,e1)<<std::endl;
	std::cout<<"Substitute y  : "<<e1.substitute(y,e1)<<" | "<<e2.substitute(y,e1)<<std::endl;

}
