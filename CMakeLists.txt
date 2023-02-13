cmake_minimum_required(VERSION 3.10)
project(catan-simulation)
add_subdirectory(pybind11)
pybind11_add_module(catan src/main.cpp)