cmake_minimum_required(VERSION 3.8)
project(CSE6140_Project)

set(CMAKE_CXX_STANDARD 11)

add_executable(CSE6140_Project branch_and_bound.cpp branch_and_bound.h
        parse_graph.h parse_graph.cpp main.cpp fileReader.h fileReader.cpp
        ILS.h ILS.cpp mstApprox.h mstApprox.cpp SA_TSP.h SA_TSP.cpp)