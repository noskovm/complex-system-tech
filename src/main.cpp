#include <iostream>
#include "problem_set_0/CircularDoublyLinkedList.h"
#include <forward_list>
#include "problem_set_1/allocator.h"

int main() {
    simple_allocator smpl_allocator;

    void* allocated_block = smpl_allocator.allocate(1000);

    return 0;
}