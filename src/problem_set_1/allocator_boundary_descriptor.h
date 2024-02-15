#ifndef COMPLEX_SYSTEM_TECH_ALLOCATOR_FIRST_FIT
#define COMPLEX_SYSTEM_TECH_ALLOCATOR_FIRST_FIT

#include <iostream>
#include <cstring>
#include <string>
//#include "../../3rdParty/allocator.h"
#include "../../3rdParty/allocator_holder.h"
#include "../../3rdParty/typename_holder.h"
#include "../../3rdParty/allocator_holder.h"
#include "../../3rdParty/logger/logger.h"
#include "../../3rdParty/logger/logger_holder.h"
#include "../../3rdParty/allocator_fit_allocation.h"
#include "m_allocator.h"

class allocator_boundary_descriptor: public m_allocator,
    // public allocator_fit_allocation,
                                    protected logger_holder
    // protected typename_holder,
    // protected allocator_holder
{
private:
    void* _trusted_memory;

private:

    logger* get_logger() const noexcept override;
    size_t get_available_block_size(void *block);
    void* get_next_link(void *block);
    void* get_prev_link(void *block);
    void set_block_size(void *block, size_t new_size);
    std::string get_typename();
    size_t get_available_block_service_block_size();
    size_t get_allocator_service_block_size();
    void** get_first_available_block_address();

public:
    allocator_boundary_descriptor(size_t requested_memory_size,
                        m_allocator *outer_allocator,
                        logger *log,
                        allocator_fit_allocation::allocation_mode allocation_mode);
    void* allocate(size_t size) override;
    void deallocate(void* target_to_dealloc) override;
    void* reallocate(void* target_to_realloc, size_t new_size) override;
};

#endif //COMPLEX_SYSTEM_TECH_ALLOCATOR_FIRST_FIT