cmake_minimum_required(VERSION 3.23)
project(ISA_Project2022 CXX)

set(CMAKE_CXX_STANDARD 14)

add_executable(ISA_Project2022 parameters.cpp error.cpp error.hpp main.cpp)
install(TARGETS ISA_Project2022)