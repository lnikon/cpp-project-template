cmake_minimum_required(VERSION 3.18)
project(cpp-project-template)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_compile_options(-Wall -Wextra -pedantic -O0 -g)
set(sources "src/main.cpp")

add_executable(main ${sources})
target_link_libraries(main ${CONAN_LIBS})
