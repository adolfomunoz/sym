#pragma once

#include "expression-multiple.h"

namespace sym {

namespace detail {

expression add_default(const std::list<expression>& exs);
expression add_default(const expression& e1, const expression& e2);

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

	expression inverse(const symbol& in, const expression& out) const override {
		std::list<expression>::const_iterator i, to_invert; bool found = false;
		for (i = expressions().begin(); i != expressions().end(); ++i) {
			if ((*i).depends_on(in)) {
				if (!found) {
					to_invert = i;
					found = true;
				} else throw not_invertible_error(*this,in);
			}	
		}
		if (!found) throw not_invertible_error(*this,in);
		else {
			expression inverted = out; 
			for (i = expressions().begin(); i != expressions().end(); ++i)
				if (i != to_invert) inverted -= (*i);
			return (*to_invert).inverse(in,inverted);
		}	
	}

	expression include(const expression& e) const {
		std::list<expression> exs = this->expressions();
		if (exs.empty()) return e;
		else {
			expression first = this->expressions().front();
			expression new_expression = (e + first);
			exs.pop_front();

			if (exs.empty()) return new_expression;
			else return sym::apply(overloaded {
					[&] (const auto& new_e) {
						return new_expression + add_default(exs);
					},
					[&] (const Addition& new_e) {
						expression internal = e + add_default(exs);
						return sym::apply(overloaded {
							[&] (const auto& internal_e) {	return add_default(first, internal); },
							[&] (const Addition& internal_a) {
								std::list<expression> internal_exs = internal_a.expressions();
								internal_exs.push_front(first);
								return add_default(internal_exs);
							}}, internal);
					}}, new_expression);
		}
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


template<typename E>
expression add(const E& e, const Addition& a) {
	return a.include(e);
}

template<typename E>
expression add(const Addition& a, const E& e) {
	return a.include(e);
}

template<typename Num>
expression add(const Constant<Num>& c, const Addition& a) {
	if (c.value()==0) return a;
	else return a.include(c);
}

template<typename Num>
expression add(const Addition& a, const Constant<Num>& c) {
	if (c.value()==0) return a;
	else return a.include(c);
}



inline expression add(const Addition& a1, const Addition& a2) {
	expression s = a1;
	for (const expression& e : a2.expressions()) s = s + e;
	return s;
}






}
}
