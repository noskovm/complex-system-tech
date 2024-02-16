#ifndef COMPLEX_SYSTEM_TECH_ALLOCATOR_H
#define COMPLEX_SYSTEM_TECH_ALLOCATOR_H

#include <cstddef>
#include <algorithm>
#include <iostream>
#include <exception>
#include <cstring>

class m_allocator {
public:
    virtual void* allocate(size_t target_size) = 0;
    virtual void deallocate(void* target_to_dealloc) = 0;
    virtual void* reallocate(void* target_to_realloc, size_t new_size) = 0;

    class memory_exception final:
        public std::exception
    {

    private:

        std::string _exception_message;

    public:

        explicit memory_exception(
            std::string exception_message);
    public:

        [[nodiscard]] char const *what() const noexcept override;

    };
};

#endif //COMPLEX_SYSTEM_TECH_ALLOCATOR_H