#pragma once

#include "detail/expression.h"
#include <iostream>

namespace sym {

namespace detail {	
	class Symbol;	
}

// Wrapper to avoid using shared pointers all the time everywhere and for easy use
class expression {
	std::shared_ptr<const detail::Expression> e_;
private:
	const std::shared_ptr<const detail::Expression>& internal_pointer() const { return e_; }
	friend class detail::Expression;
	friend class detail::Symbol;
public:
	expression(const std::shared_ptr<const detail::Expression>& e) : e_(e) { } //Should not be used in the general case
	expression() { } //Required by the visitable stuff but never used elsewhere. Should find a way to hide this from "public"

	operator bool() const { return bool(internal_pointer()); }
	bool empty() const { return bool(!internal_pointer()); }

	//Maybe add more constant constructors later on
	expression(int   i);
	expression(float f);

	std::string to_string() const { return e_->to_string(); }
	float evaluate() const { return e_->evaluate(); }
	bool depends_on(const symbol& s) const {
		return e_->depends_on(s);
	}

	expression inverse(const symbol& in, const expression& out) const {
		return e_->inverse(in, out);
	}
	
	expression derivative(const symbol& s) const { return e_->derivative(s); }
	//SUBSTITUTIONS
	expression substitute(const expression& source, const expression& replacement) const {
		if ((*this)==source) return replacement; else return expression(e_->substitute(source,replacement));
	}
	template<typename Num, typename = std::enable_if_t<std::is_floating_point_v<Num> || std::is_integral_v<Num>> > 
	expression substitute(const expression& s, const Num& n) const {
		return substitute(s,expression(n));
	}

	//ADDITIONS
	expression operator+(const expression& that) const;
	template<typename Num, typename = std::enable_if_t<std::is_floating_point_v<Num> || std::is_integral_v<Num>> > 
	expression operator+(const Num& n) const {
		return (*this)+expression(n);
	}

	//PRODUCTS
	expression operator*(const expression& that) const;
	template<typename Num, typename = std::enable_if_t<std::is_floating_point_v<Num> || std::is_integral_v<Num>> > 
	expression operator*(const Num& n) const {
		return (*this)*expression(n);
	}

	//SUBSTRACTION
	expression operator-() const {
		return (*this)*(-1);
	}
	expression operator-(const expression& that) const {
		return (*this)+(-that);
	}
	template<typename Num, typename = std::enable_if_t<std::is_floating_point_v<Num> || std::is_integral_v<Num>> > 
	expression operator-(const Num& n) const {
		return (*this)-expression(n);
	}

	//DIVISION
	expression operator/(const expression& that) const;
	template<typename Num, typename = std::enable_if_t<std::is_floating_point_v<Num> || std::is_integral_v<Num>> > 
	expression operator/(const Num& n) const { return (*this)/expression(n); }

	//This is for comfort but somehow breaks the "functional" aspect of everything. Still, it is used (although it is
	//not optimal).
	template<typename That>
	expression& operator+=(const That& that) {
		(*this) = (*this) + that;
		return (*this);
	}
	template<typename That>
	expression& operator-=(const That& that) {
		(*this) = (*this) - that;
		return (*this);
	}
	template<typename That>
	expression& operator*=(const That& that) {
		(*this) = (*this) * that;
		return (*this);
	}
	template<typename That>
	expression& operator/=(const That& that) {
		(*this) = (*this) / that;
		return (*this);
	}
	
	//APPLY (VISITOR STUFF FOR DOUBLE DISPATCH). WORKS SIMILAR TO STD::VISIT BUT WITH INHERITANCE.
	template<typename F>
	friend auto apply(const F& f, const expression& e);

	//EQUALITY CHECK.
	//    False if different types.
	//    Defined in Symbol so it compares pointers.
	//    Default behavior in the rest.
	//    Maybe in the future compare propperly disordered additions and products (need to redefine the equality test in Addition and Product)
	bool operator==(const expression& that) const;
	bool operator!=(const expression& that) const { return !((*this)==that); }
};



template<typename Num, typename = std::enable_if_t<std::is_floating_point_v<Num> || std::is_integral_v<Num>> >
expression operator+(const Num& a, const expression& b) { return b+a; }
template<typename Num, typename = std::enable_if_t<std::is_floating_point_v<Num> || std::is_integral_v<Num>> >
expression operator-(const Num& a, const expression& b) { return -b+a; }
template<typename Num, typename = std::enable_if_t<std::is_floating_point_v<Num> || std::is_integral_v<Num>> >
expression operator*(const Num& a, const expression& b) { return b*a; }
template<typename Num, typename = std::enable_if_t<std::is_floating_point_v<Num> || std::is_integral_v<Num>> >
expression operator/(const Num& a, const expression& b) { return expression(a)/b; }

expression pow(const expression& base, const expression& exponent);

template<typename Num, typename = std::enable_if_t<std::is_floating_point_v<Num> || std::is_integral_v<Num>> >
expression pow(const expression& base, const Num& n) {
	return pow(base, expression(n));
}

template<typename Num, typename = std::enable_if_t<std::is_floating_point_v<Num> || std::is_integral_v<Num>> >
expression pow(const Num& n, const expression& exponent) {
	return pow(expression(n), exponent);
}

expression log(const expression& base, const expression& number);
expression ln(const expression& number);

expression sin(const expression& e);
expression cos(const expression& e);
expression asin(const expression& e);
expression acos(const expression& e);

template<typename F>
auto apply(const F& f, const expression& e) { return e.e_->apply(f); }

//DOUBLE DISPACTH
template<typename F>
auto apply(const F& f, const expression& e1, const expression& e2) {
	return apply([&] (const auto& e1_applied) { return apply([&] (const auto& e2_applied) { return f(e1_applied, e2_applied); },e2); }, e1);
}

std::ostream& operator<<(std::ostream& os, const expression& e);

}
