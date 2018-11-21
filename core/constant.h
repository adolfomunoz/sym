#pragma once

#include <memory>
#include <string>

namespace sym {

template<typename Num>
class Constant : public Expression {
	Num c;
public:
	Constant(const Num& c) : c(c) { }

	std::string to_string() const override { return std::to_string(c); }
	float eval() const override { return float(c); }
	std::shared_ptr<Expression> subs(const Symbol& symbol, const std::shared_ptr<Expression>& e) const override {
		return std::make_shared<Constant>(c); 
	}
};


}
