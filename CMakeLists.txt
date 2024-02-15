cmake_minimum_required(VERSION 3.22)
project(complex_system_tech)

set(CMAKE_CXX_STANDARD 14)



include_directories(src
                    src/problem_set_0
                    src/problem_set_1
                    3rdParty/nlohmann
                    3rdParty/logger)


add_executable(complex_system_tech
        src/problem_set_0/CircularDoublyLinkedList.cpp
        src/problem_set_0/CircularDoublyLinkedList.h
        src/main.cpp
        src/problem_set_1/m_allocator.h
        src/problem_set_1/simple_allocator.h
        src/problem_set_1/simple_allocator.cpp
        src/problem_set_1/allocator_boundary_descriptor.h
        src/problem_set_1/allocator_boundary_descriptor.cpp
        3rdParty/allocator_holder.cpp
        3rdParty/allocator_sorted_list.cpp
        3rdParty/allocator.cpp
        3rdParty/not_implemented.cpp
        3rdParty/operation_not_supported.cpp
        3rdParty/logger/logger_builder_concrete.cpp
        3rdParty/logger/logger_builder.cpp
        3rdParty/logger/logger_concrete.cpp
        3rdParty/logger/logger_holder.cpp
        3rdParty/logger/logger.cpp)

add_subdirectory(3rdParty/json)


target_link_libraries(complex_system_tech PRIVATE nlohmann_json::nlohmann_json)