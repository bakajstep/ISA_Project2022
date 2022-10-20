cmake_minimum_required(VERSION 3.23)
project(ISA_Project2022)

set(CMAKE_CXX_STANDARD 14)
SET(GCC_COVERAGE_LINK_FLAGS    "-lpcap")

add_executable(ISA_Project2022 parameters.cpp error.cpp error.hpp main.cpp flows.cpp flows.h udp_client.cpp udp_client.h)
install(TARGETS ISA_Project2022)
include(FindPCAP.cmake)
target_link_libraries(ISA_Project2022 ${PCAP_LIBRARY})

