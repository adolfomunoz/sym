#include <sym.h>
#include <iostream>

int main(int argc, char** argv) {
	sym::symbol x("x");
	sym::symbol y("y");
	sym::expression ex1 = 2*x*x + 3*y*y + x*y - x + 25;

	std::cout<<"E1(x,y) = "<<ex1<<std::endl;
	std::cout<<"dE1/dx  = "<<ex1.derivative(x)<<std::endl;
	std::cout<<"dE1/dy  = "<<ex1.derivative(y)<<std::endl;
}
