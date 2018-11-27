#include <sym.h>
#include <iostream>

int main(int argc, char** argv) {
	sym::symbol x("x");
	sym::symbol y("y");
	sym::expression ex1 = 3*(x + 2) - y*(2 + y);

	std::cout<<"E1(x,y) = "<<ex1<<std::endl;
	std::cout<<"E1(3,4) = "<<ex1.substitute(x,3).substitute(y,4)<<std::endl;
}
