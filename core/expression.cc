#include "expression.h"
#include "detail/constant.h"
#include "detail/addition.h"
#include "detail/product.h"
#include "detail/symbol.h"
#include "detail/expression-binary.h"

namespace sym {

expression::expression(int i)   : expression::expression(std::make_shared<detail::Constant<int>>(i)) { }
expression::expression(float f) : expression::expression(std::make_shared<detail::Constant<float>>(f)) { }
 
struct EqualityTest {
	template<typename T>
	bool operator()(const T& t1, const T& t2) const { return t1==t2; }
	template<typename T, typename U>
	bool operator()(const T& t, const U& u) const { return false; }
};

bool expression::operator==(const expression& that) const {
	return apply(EqualityTest(), *this, that);
}

expression expression::operator+(const expression& that) const {
	return apply([] (const auto& e1, const auto& e2) { return detail::add(e1,e2); },*this,that);
}

expression expression::operator*(const expression& that) const {
	return apply([] (const auto& e1, const auto& e2) { return detail::multiply(e1,e2); },*this,that);
}

std::ostream& operator<<(std::ostream& os, const expression& e) {
	os<<e.to_string();
	return os;
}

};
