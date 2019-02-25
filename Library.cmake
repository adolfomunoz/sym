####################################################################
# SOURCE FILES
####################################################################
set(srcs_sym 
	${CMAKE_CURRENT_LIST_DIR}/core/expression.cc
	${CMAKE_CURRENT_LIST_DIR}/core/detail/expression.cc
	${CMAKE_CURRENT_LIST_DIR}/core/detail/symbol.cc
)

####################################################################
# LIBRARY TARGETS
####################################################################
add_library(sym STATIC ${srcs_sym})

