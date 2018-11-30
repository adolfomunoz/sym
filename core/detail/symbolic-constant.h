#pragma once

#include "symbol.h"

namespace sym {

namespace detail {
class SymbolicConstant : public VisitableDerived<SymbolicConstant,Symbol> {
	float value_;
public:
	SymbolicConstant(const std::string& name, float value) : VisitableDerived<SymbolicConstant,Symbol>(name), value_(value) { }
	float value() const { return value_; }

	float evaluate() const override { return value(); }
};

}
}
