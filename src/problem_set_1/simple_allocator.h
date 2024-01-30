#ifndef COMPLEX_SYSTEM_TECH_SIMPLE_ALLOCATOR_H
#define COMPLEX_SYSTEM_TECH_SIMPLE_ALLOCATOR_H

#include <cstddef>
#include <algorithm>
#include <cstring>
#include <iostream>
#include "allocator.h"
#include "../../3rdParty/logger/logger.h"

class simple_allocator: public allocator {
public:
    void* allocate(size_t target_size) override;
    void deallocate(void* target_to_dealloc) override;
    void* reallocate(void* target_to_realloc, size_t new_size) override;
};

size_t get_occupied_block_size(void* block);


#endif //COMPLEX_SYSTEM_TECH_SIMPLE_ALLOCATOR_H