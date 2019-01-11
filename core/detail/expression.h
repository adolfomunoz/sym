#pragma once

#include <memory>
#include <string>
#include <exception>
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
	class Power;
	class Logarithm;
	class Sin;
	class Cos;
	class Asin;
	class Acos;
	class Id;

	class Expression : public VisitableBase<Expression, Visitor<Constant<int>,Constant<float>,Addition,Product,Power,Logarithm,Symbol,Sin,Cos,Asin,Acos,Id>>, public std::enable_shared_from_this<Expression> {
	public:
		virtual std::string to_string() const = 0;
		virtual float evaluate() const = 0;	
		virtual bool depends_on(const symbol& s) const = 0;
		virtual expression derivative(const symbol& s) const = 0;
		virtual expression inverse(const symbol& in, const expression& out) const; 
		expression to_expression() const;
		operator expression() const;

		//By default it does nothing (returns itself)
		virtual expression substitute(const expression& source, const expression& replacement) const;
		virtual int precedence() const {  return 10; } //Useful for showing parenthesis when the operator precedence is different than the tree-like structure
	protected:
		std::string subexpression_to_string(const expression& sub) const;
	};
}

}
