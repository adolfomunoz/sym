#include "expression.h"
#include "detail/constant.h"
#include "detail/addition.h"
#include "detail/product.h"
#include "detail/symbol.h"
#include "detail/power.h"
#include "detail/logarithm.h"
#include "detail/sin.h"
#include "detail/cos.h"
#include "detail/asin.h"
#include "detail/acos.h"
#include "detail/id.h"
#include "constants.h"

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

expression pow(const expression& base, const expression& exponent) {
	return apply([] (const auto& e1, const auto& e2) { return detail::pow(e1,e2); },base,exponent);
}

expression log(const expression& base, const expression& number) {
	return apply([] (const auto& e1, const auto& e2) { return detail::log(e1,e2); },base,number);
}

expression sin(const expression& ex) {
	return apply([] (const auto& e) { return detail::sin(e); },ex);
}

expression cos(const expression& ex) {
	return apply([] (const auto& e) { return detail::cos(e); },ex);
}

expression asin(const expression& ex) {
	return apply([] (const auto& e) { return detail::asin(e); },ex);
}

expression acos(const expression& ex) {
	return apply([] (const auto& e) { return detail::acos(e); },ex);
}

expression expression::operator/(const expression& that) const {
	return (*this)*pow(that,-1);
}

expression ln(const expression& number) {
	return log(sym::e,number);
}

expression id(const expression& e) {
	return expression(std::make_shared<detail::Id>(e));
}


std::ostream& operator<<(std::ostream& os, const expression& e) {
	os<<e.to_string();
	return os;
}

};
