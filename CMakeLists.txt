cmake_minimum_required(VERSION 3.31)

project(algos
    VERSION 0.1
    DESCRIPTION "Practice of common algorithms and datastructures"
    HOMEPAGE_URL https://github.com/lightspeedbriefs/algos
    LANGUAGES CXX)

add_executable(algos_demo src/main.cpp)

set_property(TARGET algos_demo PROPERTY CXX_STANDARD 20)
set_property(TARGET algos_demo PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET algos_demo PROPERTY CXX_EXTENSIONS OFF)
set_property(TARGET algos_demo PROPERTY COMPILE_WARNING_AS_ERROR ON)
set_property(TARGET algos_demo PROPERTY EXPORT_COMPILE_COMMANDS ON)

target_compile_options(algos_demo PRIVATE -Wall -Wextra)
target_compile_definitions(algos_demo PRIVATE GLIBCXX_ASSERTIONS)

# -- Unit tests (Boost.Test)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_executable(avl_tests tests/TestAvlTree.cpp)

set_property(TARGET avl_tests PROPERTY CXX_STANDARD 20)
set_property(TARGET avl_tests PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET avl_tests PROPERTY CXX_EXTENSIONS OFF)
set_property(TARGET avl_tests PROPERTY COMPILE_WARNING_AS_ERROR ON)
set_property(TARGET avl_tests PROPERTY EXPORT_COMPILE_COMMANDS ON)

target_compile_options(avl_tests PRIVATE -Wall -Wextra)
target_compile_definitions(avl_tests PRIVATE GLIBCXX_ASSERTIONS)
target_include_directories(avl_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(avl_tests PRIVATE Boost::unit_test_framework)

enable_testing()
add_test(NAME avl_tests COMMAND avl_tests)
