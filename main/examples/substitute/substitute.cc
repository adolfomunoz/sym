#include <sym.h>
#include <iostream>

void show(const sym::expression& e, const sym::expression& source, const sym::expression& replacement) {
	std::cout<<"Expression: "<<e<<std::endl;
	std::cout<<"Substitute "<<source<<" with "<<replacement<<" : "<<e.substitute(source,replacement)<<std::endl;
	std::cout<<std::endl;	
}


int main(int argc, char** argv) {
	sym::symbol x("x");
	sym::symbol y("y");
	show(x+1,x,y);
	show(x+1,x,3);
	show(x*y+1,x*y,1);
}
