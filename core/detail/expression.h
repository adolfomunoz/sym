#pragma once

#include <memory>
#include <string>
#include "../../utils/visitor.h"

namespace sym {

class symbol;
class expression;

namespace detail {
	template<typename Num>
	class Constant;
	class Addition;
	class Product;
	class Symbol;

	class Expression : public VisitableBase<Expression, Visitor<Constant<int>,Constant<float>,Addition,Product,Symbol>>, public std::enable_shared_from_this<Expression> {
	public:
		virtual std::string to_string() const = 0;
		virtual float evaluate() const = 0;	
		virtual bool depends_on(const symbol& s) const = 0;
		virtual expression substitute(const symbol& s, const expression& e) const = 0;
		virtual expression derivative(const symbol& s) const = 0;
		expression to_expression() const;
		operator expression() const;
		//Has a default behavior but can be optimized under specific circumstances
		virtual expression add_to(const expression& that) const;
		//Has a default behavior but can be optimized under specific circumstances
		virtual expression multiply_by(const expression& that) const;

	};
}

}
