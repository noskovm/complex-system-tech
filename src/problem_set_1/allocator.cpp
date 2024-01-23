#include "allocator.h"

void* simple_allocator::allocate(size_t target_size) {
    return ::operator new(target_size);
}

void simple_allocator::deallocate(void* target_to_dealloc) {
    ::operator delete(target_to_dealloc);
}

void* simple_allocator::reallocate(void* target_to_realloc, size_t new_size) {
   //
}

