#pragma once

#include "expression-multiple.h"

namespace sym {

namespace detail {

expression multiply_default(const std::list<expression>& exs);
expression multiply_default(const expression& e1, const expression& e2);

class Product : public VisitableDerived<Product,ExpressionMultiple> {
public:
	using VisitableDerived<Product,ExpressionMultiple>::VisitableDerived;
	
	float evaluate() const override { 
		return transform_accumulate(0.0f, [] (const expression& ex) { return ex.evaluate(); }, std::multiplies<float>());
	}

	expression substitute(const symbol& s, const expression& e) const override {
		return transform_accumulate_first([&] (const expression& ex) { return ex.substitute(s,e); }, std::multiplies<expression>());
	}

	expression derivative(const symbol& s) const override {
		expression der = 0;
		std::list<expression> exs = expressions();
		expression front_product = 1;
		while (!exs.empty()) {
			expression front = exs.front();
			exs.pop_front();
			der = der + front_product*front.derivative(s)*multiply_default(exs);
			front_product = front_product * front;
		}
		return der;
	}

	expression include(const expression& e) const {
		std::list<expression> exs = this->expressions();
		if (exs.empty()) return e;
		else {
			expression last = this->expressions().back();
			expression new_expression = (last * e);
			exs.pop_back();

			if (exs.empty()) return new_expression;
			else return sym::apply(overloaded {
					[&] (const auto& new_e) {
						return multiply_default(exs) * new_expression;
					},
					[&] (const Product& new_e) {
						expression internal = add_default(exs) * e;
						return sym::apply(overloaded {
							[&] (const auto& internal_e) {	return multiply_default(internal, last); },
							[&] (const Product& internal_a) {
								std::list<expression> internal_exs = internal_a.expressions();
								internal_exs.push_back(last);
								return multiply_default(internal_exs);
							}}, internal);
					}}, new_expression);
		}
	}


	int precedence() const override { return 4; }
};

inline expression multiply_default(std::initializer_list<expression> exs) {
	if (exs.size() == 0) return expression(1);
	else if (exs.size() == 1) return *(exs.begin());
	else return expression(std::make_shared<Product>(exs));
}

inline expression multiply_default(const std::list<expression>& exs) {
	if (exs.empty()) return expression(1);
	else if (exs.size() == 1) return *(exs.begin());
	else return expression(std::make_shared<Product>(exs));
}

inline expression multiply_default(std::list<expression>&& exs) {
	if (exs.empty()) return expression(1);
	else if (exs.size() == 1) return *(exs.begin());
	else return expression(std::make_shared<Product>(std::forward<std::list<expression>>(exs)));
}

inline expression multiply_default(const expression& e1, const expression& e2) {
	return multiply_default(std::initializer_list<expression>({e1,e2}));
}

//Default multiply for double dispatch. Must create new "multiply" versions for specific subtypes of detail::Expression
template<typename E1, typename E2>
expression multiply(const E1& e1, const E2& e2) {
	return multiply_default(e1,e2);
}

template<typename E>
expression multiply(const E& e, const Product& p) {
	return p.include(e);
}

template<typename E>
expression multiply(const Product& p, const E& e) {
	return p.include(e);
}

inline expression multiply(const Product& p1, const Product& p2) {
	expression s = p1;
	for (const expression& e : p2.expressions()) s = s * e;
	return s;
}

//MIXTURES OF ADDITION WITH PRODUCTS

//If they are equal, we incorporate a product (2*)
template<typename E>
expression add(const E& e1, const E& e2) {
	if (e1 == e2) return 2*e1.to_expression();
	else return add_default(e1, e2);
}

struct ConstantGetter {
	template<typename T>
	expression operator()(const T& t) const { return expression(); }
	template<typename Num>
	expression operator()(const Constant<Num>& c) const { return c.to_expression(); }
};

template<typename E, typename = std::enable_if_t<!std::is_base_of_v<Addition,E>>>
expression add(const Product& p, const E& e) {
	if (p.expressions().size() != 2) return add_default(p,e);
	else {
		std::list<expression> exs = p.expressions();
		expression constant;
		for (std::list<expression>::iterator i = exs.begin(); (i != exs.end()) && constant.empty(); ++i) 
			if (constant = apply(ConstantGetter(),*i)) exs.erase(i);
	
		//ONLY ONE EXPRESSION LEFT
		if ((constant) && (exs.front() == e)) return (constant + 1)*e;
		else return add_default(p,e);
	}
}

template<typename Num>
expression add(const Product& p, const Constant<Num>& c) {
	if (c.value()==0) return p;
	else return add_default(p,c);
}

template<typename Num>
expression add(const Constant<Num>& c, const Product& p) {
	if (c.value()==0) return p;
	else return add_default(p,c);
}

template<typename E, typename = std::enable_if_t<!std::is_base_of_v<Addition,E>>>
expression add(const E& e, const Product& p) {
	return add(p,e);
}

inline expression add(const Product& p1, const Product& p2) {
	if ((p1.expressions().size() != 2) || (p2.expressions().size() != 2)) return add_default(p1,p2);
	else {
		std::list<expression> exs1 = p1.expressions();
		expression constant1;
		for (std::list<expression>::iterator i = exs1.begin(); (i != exs1.end()) && constant1.empty(); ++i) 
			if (constant1 = apply(ConstantGetter(),*i)) exs1.erase(i);
		std::list<expression> exs2 = p2.expressions();
		expression constant2;
		for (std::list<expression>::iterator i = exs2.begin(); (i != exs2.end()) && constant2.empty(); ++i) 
			if (constant2 = apply(ConstantGetter(),*i)) exs2.erase(i);
	
		//ONLY ONE EXPRESSION LEFT
		if ((constant1) && (constant2) && (exs1.front() == exs2.front())) return (constant1 + constant2)*exs1.front();
		else return add_default(p1,p2);
	}
}


}
}
