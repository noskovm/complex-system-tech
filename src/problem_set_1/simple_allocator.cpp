#include "simple_allocator.h"

void* simple_allocator::allocate(size_t target_size) {
    void* new_block = ::operator new(target_size + sizeof(size_t));
    size_t* block_for_size = reinterpret_cast<size_t *>(new_block);
    block_for_size[0] = target_size;
    void* block = reinterpret_cast<void*>(block_for_size);
    return block;
}

void simple_allocator::deallocate(void* target_to_dealloc) {
    ::operator delete(target_to_dealloc);
}

void* simple_allocator::reallocate(void* target_to_realloc, size_t new_size) {
    void* new_block = allocate(new_size);
    size_t move_size = std::min(new_size, get_occupied_block_size(target_to_realloc));
    memcpy(new_block, target_to_realloc, move_size);
    deallocate(target_to_realloc);
    return new_block;
}

size_t get_occupied_block_size(void* block) {
    return *reinterpret_cast<size_t*>(block);
}
