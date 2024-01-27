cmake_minimum_required(VERSION 3.22)
project(complex_system_tech)

set(CMAKE_CXX_STANDARD 14)

include_directories(src
                    src/problem_set_0
                    src/problem_set_1)

add_executable(complex_system_tech
        src/problem_set_0/CircularDoublyLinkedList.cpp
        src/problem_set_0/CircularDoublyLinkedList.h
        src/main.cpp
        src/problem_set_1/allocator.cpp
        src/problem_set_1/allocator.h)
