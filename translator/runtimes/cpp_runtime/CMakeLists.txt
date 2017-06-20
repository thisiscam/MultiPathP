cmake_minimum_required(VERSION 3.8)

find_package(PkgConfig REQUIRED)
pkg_search_module(sylvan REQUIRED sylvan)

set(SRCS
    main.cpp
)

add_executable(main ${SRCS})
target_compile_features(main PRIVATE cxx_auto_type)
target_include_directories(main PUBLIC ${sylvan_INCLUDE_DIRS})
target_compile_options(main PUBLIC ${sylvan_CFLAGS_OTHER})

add_library(p_runtime INTERFACE)
target_include_directories(p_runtime INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
