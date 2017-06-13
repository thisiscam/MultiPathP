cmake_minimum_required(VERSION 3.8)

set(SRCS
	main.cpp
)

add_executable(main ${SRCS})
target_compile_features(main PRIVATE cxx_auto_type)
