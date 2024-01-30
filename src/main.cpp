#include <iostream>

#include "problem_set_1/allocator.h"
#include "problem_set_1/simple_allocator.h"

int main() {

    simple_allocator smpl_allocator;
    
    void* block = smpl_allocator.allocate(100000);
    void* bigger_block = smpl_allocator.reallocate(block, 10);

    void* another_block = smpl_allocator.allocate(100);
    void* bbbblock = smpl_allocator.reallocate(another_block, 2000000);

    std::cout << "Hello World";
    
    return 0;
}