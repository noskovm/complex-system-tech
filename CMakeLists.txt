cmake_minimum_required(VERSION 3.26)
project(complex_system_tech)

set(CMAKE_CXX_STANDARD 14)

include_directories(src
                    src/problem_set_0)

add_executable(complex_system_tech
        src/problem_set_0/CircularDoublyLinkedList.cpp
        src/problem_set_0/CircularDoublyLinkedList.h
        src/main.cpp)
