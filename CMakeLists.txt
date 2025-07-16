cmake_minimum_required(VERSION 3.30)
project(graph-algorithms-comparison)

set(CMAKE_CXX_STANDARD 20)

add_executable(graph-algorithms-comparison main.cpp
        aLgorithms.h
	timer.cpp
	timer.h
	modes.h
)
