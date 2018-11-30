#pragma once

#include "expression.h"
#include "detail/symbolic-constant.h"

namespace sym {

const expression e = expression(std::make_shared<detail::SymbolicConstant>("e",2.718281828459045235360287471352f));
const expression pi = expression(std::make_shared<detail::SymbolicConstant>("\\pi",3.141592653589793238462643383279502884197169399375105820974944592307816406286f));

}
