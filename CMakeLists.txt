cmake_minimum_required(VERSION 3.25)

project(
    test
    VERSION 0.0.1
    LANGUAGES CXX)

find_package(fmt REQUIRED)
find_package(pybind11 REQUIRED)

pybind11_add_module(test src/test.cpp)
target_link_libraries(test PRIVATE fmt)