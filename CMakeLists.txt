cmake_minimum_required(VERSION 3.10)
project(sym)

include(Compiler)
include(Paths)
include(Library)

####################################################################
# USEFUL FUNCTIONS
####################################################################
function(add_subdirectories_and_compile directory)
	file(GLOB children ${directory}/*)
	get_filename_component(dir ${directory} NAME)
	foreach(fullchild ${children})
		get_filename_component(child ${fullchild} NAME)
		if(IS_DIRECTORY ${fullchild})
			if (EXISTS ${fullchild}/${child}.cc)
				add_executable(${CMAKE_PROJECT_NAME}-${dir}-${child} ${fullchild}/${child}.cc)
				target_link_libraries(${CMAKE_PROJECT_NAME}-${dir}-${child} sym)
			else()
				add_subdirectory(${fullchild})
			endif()
		endif()
	endforeach()
endfunction(add_subdirectories_and_compile)

####################################################################
# EXECUTABLE TARGETS
####################################################################
add_subdirectories_and_compile(main/examples)
add_subdirectories_and_compile(main/test)
