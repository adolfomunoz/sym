#pragma once

#include "detail/expression.h"
#include <iostream>

namespace sym {

namespace detail {	class Symbol;	}

// Wrapper to avoid using shared pointers all the time everywhere and for easy use
class expression {
	std::shared_ptr<const detail::Expression> e_;
private:
	const std::shared_ptr<const detail::Expression>& internal_pointer() const { return e_; }
	friend class detail::Symbol;
protected:
	expression(const std::shared_ptr<const detail::Expression>& e) : e_(e) { }
public:
	//Maybe add more constant constructors later on
	expression(int   i);
	expression(float f);

	std::string to_string() const { return e_->to_string(); }
	float evaluate() const { return e_->evaluate(); }
	bool depends_on(const symbol& s) const {
		return e_->depends_on(s);
	}
	
	expression derivative(const symbol& s) const { return e_->derivative(s); }
	//SUBSTITUTIONS
	expression substitute(const symbol& s, const expression& e) const {
		return expression(e_->substitute(s,e));
	}
	template<typename Num, typename = std::enable_if_t<std::is_floating_point_v<Num> || std::is_integral_v<Num>> > 
	expression substitute(const symbol& s, const Num& n) const {
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


};

template<typename Num, typename = std::enable_if_t<std::is_floating_point_v<Num> || std::is_integral_v<Num>> >
expression operator+(const Num& a, const expression& b) { return b+a; }
template<typename Num, typename = std::enable_if_t<std::is_floating_point_v<Num> || std::is_integral_v<Num>> >
expression operator*(const Num& a, const expression& b) { return b*a; }

std::ostream& operator<<(std::ostream& os, const expression& e);

}
