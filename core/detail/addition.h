#pragma once

#include "expression-multiple.h"

namespace sym {

namespace detail {
class Addition : public VisitableDerived<Addition,ExpressionMultiple> {
public:
	using VisitableDerived<Addition,ExpressionMultiple>::VisitableDerived;

	std::string operator_symbol() const override { return std::string(" + "); }
	
	float evaluate() const override { 
		return transform_accumulate(0.0f, [] (const expression& ex) { return ex.evaluate(); }, std::plus<float>());
	}
	
	expression substitute(const symbol& s, const expression& e) const override {
		return transform_accumulate_first([&] (const expression& ex) { return ex.substitute(s,e); }, std::plus<expression>());
	}

	expression derivative(const symbol& s) const override {
		return transform_accumulate_first([&] (const expression& ex) { return ex.derivative(s); }, std::plus<expression>());
	}

	expression insert(const expression& e) const {
		return accumulate(e, std::plus<expression>());
	}

	int precedence() const override { return 3; }
};

inline expression add_default(std::initializer_list<expression> exs) {
	if (exs.size() == 0) return expression(0);
	else if (exs.size() == 1) return *(exs.begin());
	else return expression(std::make_shared<Addition>(exs));
}

inline expression add_default(const std::list<expression>& exs) {
	if (exs.empty()) return expression(0);
	else if (exs.size() == 1) return *(exs.begin());
	else return expression(std::make_shared<Addition>(exs));
}

inline expression add_default(std::list<expression>&& exs) {
	if (exs.empty()) return expression(0);
	else if (exs.size() == 1) return *(exs.begin());
	else return expression(std::make_shared<Addition>(std::forward<std::list<expression>>(exs)));
}


inline expression add_default(const expression& e1, const expression& e2) {
	return add_default(std::initializer_list<expression>({e1,e2}));
}

//Default add for double dispatch. Must create new "add" versions for specific subtypes of detail::Expression
template<typename E1, typename E2>
expression add(const E1& e1, const E2& e2) {
	return add_default(e1, e2);
}

//If left to right associativity then we explore it otherwise when accumulating
template<typename E>
expression add(const E& e, const Addition& a) {
	return a.accumulate(e.to_expression(), std::plus<expression>());
}


template<typename E>
expression add(const Addition& a, const E& e) {
	std::list<expression> exs = a.expressions();
	if (exs.empty()) return e;
	else {
		expression last = a.expressions().back();
		expression new_expression = (last + e);
		exs.pop_back();

		if (exs.empty()) return new_expression;
		else return apply(overloaded {
				[&] (const auto& new_e) {
					return add_default(exs) + new_expression;
				},
				[&] (const Addition& new_e) {
					expression internal = add_default(exs) + e;
					return apply(overloaded {
						[&] (const auto& internal_e) {	return add_default(internal_e, last); },
						[&] (const Addition& internal_a) {
							std::list<expression> internal_exs = internal_a.expressions();
							internal_exs.push_back(last);
							return add_default(internal_exs);
						}}, internal);
				}}, new_expression);
	}
}

inline expression add(const Addition& a1, const Addition& a2) {
	expression s = a1;
	for (const expression& e : a2.expressions()) s = s + e;
	return s;
}






}
}
