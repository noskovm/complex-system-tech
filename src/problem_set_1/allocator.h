#ifndef COMPLEX_SYSTEM_TECH_ALLOCATOR_H
#define COMPLEX_SYSTEM_TECH_ALLOCATOR_H

#include <cstddef>
#include <algorithm>
#include <iostream>

class allocator {
public:
    virtual void* allocate(size_t target_size) = 0;
    virtual void deallocate(void* target_to_dealloc) = 0;
    virtual void* reallocate(void* target_to_realloc, size_t new_size) = 0;
};

class simple_allocator: public allocator {
public:
    void* allocate(size_t target_size) override;
    void deallocate(void* target_to_dealloc) override;
    void* reallocate(void* target_to_realloc, size_t new_size) override;
};

#endif //COMPLEX_SYSTEM_TECH_ALLOCATOR_H
