#pragma once

#include "../expression.h"
#include <list>
#include <functional>
#include <algorithm>
#include <numeric>

namespace sym {

namespace detail {
class ExpressionMultiple : public VisitableDerived<ExpressionMultiple,Expression> {
	std::list<expression> exs_;
public:
	ExpressionMultiple(std::initializer_list<expression> l) : exs_(l) { }
	ExpressionMultiple(const std::list<expression>& l) : exs_(l) { }
	ExpressionMultiple(std::list<expression>&& l) : exs_(std::forward<std::list<expression>>(l)) { }
	const std::list<expression>& expressions() const { return exs_; }

	template<typename T, typename BinaryOperator>
	T accumulate(T init, BinaryOperator op) const {
		return std::accumulate(expressions().begin(), expressions().end(), init, op);
	}

	template<typename T, typename UnaryOperator, typename BinaryOperator> 
	T transform_accumulate(T init, UnaryOperator uop, BinaryOperator bop) const {
		return accumulate(init, [&] (T t, const expression& e) { return bop(t,uop(e)); });
	}

	bool depends_on(const symbol& s) const override {
		return transform_accumulate(false, [&s] (const expression& e) { return e.depends_on(s); }, std::logical_or<bool>());
		//PARALLELISM TS IS NOT READY. COME BACK HERE WHEN IT IS.
//		return std::transform_reduce(expressions().begin(), expressions().end(), [&s] (const expression& e) { return e.depends_on(s); }, false, std::logical_or<bool>());
	}

	template<typename UnaryOperator, typename BinaryOperator> 
	auto transform_accumulate_first(UnaryOperator uop, BinaryOperator bop) const {
		return std::accumulate(std::next(expressions().begin()), expressions().end(), uop(expressions().front()), [&] (const auto& t, const expression& e) { return bop(t,uop(e)); });
	}

	virtual std::string operator_symbol() const { return std::string(""); }

	std::string to_string() const override {
		return transform_accumulate_first([&] (const expression& ex) { return this->subexpression_to_string(ex); },
				[&] (const std::string& s1, const std::string& s2) { return s1 + this->operator_symbol() + s2; });
	}

	//This is a problem, we have to define it
	bool operator==(const ExpressionMultiple& that) const { return false; }

};

}
}
