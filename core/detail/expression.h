#pragma once

#include <memory>
#include <string>

namespace sym {

class symbol;
class expression;

namespace detail {
	class Expression {
	public:
		virtual std::string to_string() const = 0;
		virtual float evaluate() const = 0;	
		virtual expression substitute(const symbol& s, const expression& e) const = 0;
	};
}

}
