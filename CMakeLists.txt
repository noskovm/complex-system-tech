cmake_minimum_required(VERSION 3.26)
project(complex_system_tech)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(complex_system_tech
        src/DoublyLinkedCircularList.cpp
        src/DoublyLinkedCircularList.h
        src/main.cpp)
