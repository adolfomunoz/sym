#include <sym.h>
#include <iostream>

int main(int argc, char** argv) {
	sym::ex constant(std::make_shared<sym::Constant<int>>(16));
	sym::Symbol x("x");
	sym::Symbol y("y");

	sym::ex what(std::make_shared<sym::Symbol>(x));

	std::cout<<constant.to_string()<<std::endl;
	std::cout<<what.to_string()<<std::endl;
	std::cout<<constant.subs(x,constant).to_string()<<std::endl;
	std::cout<<what.subs(x,constant).to_string()<<std::endl;
	std::cout<<what.subs(y,constant).to_string()<<std::endl;
}
