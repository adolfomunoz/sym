#include <sym.h>
#include <iostream>

std::string show_and_evaluate(const sym::expression& ex) {
	try {
		float f = ex.evaluate();
		return ex.to_string()+" = "+std::to_string(f);
	} catch(const sym::symbol_evaluation_error& error) {
		return ex.to_string()+" cannot be evaluated due to symbol "+error.name();
	}
}

int main(int argc, char** argv) {
	sym::symbol x("x");
	sym::symbol y("y");
	sym::expression ex = x + y + 16;

	std::cout<<show_and_evaluate(ex)<<std::endl;
	std::cout<<show_and_evaluate(ex.substitute(x,-3))<<std::endl;
	std::cout<<show_and_evaluate(ex.substitute(x,-3).substitute(y,29))<<std::endl;
	std::cout<<show_and_evaluate(ex.substitute(y,y+3))<<std::endl;
}
