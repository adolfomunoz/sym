#include <sym.h>
#include <iostream>

void test(const sym::expression& e) {
	std::cout<<e<<" = ";
	try {
		std::cout<<e.evaluate()<<std::endl;
	} catch (const sym::symbol_evaluation_error& error) {
		std::cout<<error.what()<<std::endl;
	}
}

int main(int argc, char** argv) {
	sym::symbol x("x");
	test(x);
	test(x.substitute(x,2));
	test(x.substitute(x,0.2f));
	test(2*x);
	test((2*x).substitute(x,2));
	test((2*x).substitute(x,0.2f));
}
