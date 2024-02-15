#ifndef COMPLEX_SYSTEM_TECH_SIMPLE_ALLOCATOR_H
#define COMPLEX_SYSTEM_TECH_SIMPLE_ALLOCATOR_H

#include <cstddef>
#include <algorithm>
#include <cstring>
#include <iostream>
#include "m_allocator.h"
#include "../../3rdParty/logger/logger.h"
#include "../../3rdParty/allocator_fit_allocation.h"

class simple_allocator: public m_allocator {
    logger *log;
public:
    simple_allocator(logger *logger = nullptr);
    void* allocate(size_t target_size) override;
    void deallocate(void* target_to_dealloc) override;
    void* reallocate(void* target_to_realloc, size_t new_size) override;
};

size_t get_occupied_block_size(void* block);

#endif //COMPLEX_SYSTEM_TECH_SIMPLE_ALLOCATOR_H