#include <iostream>

#include "problem_set_1/m_allocator.h"
#include "../../3rdParty/logger/logger_builder.h"
#include "../../3rdParty/logger/logger_builder_concrete.h"
#include "problem_set_1/simple_allocator.h"
#include "problem_set_1/allocator_boundary_descriptor.h"

int main() {

    logger_builder *builder = new logger_builder_concrete();
    auto *logger = builder
        ->add_console_stream(logger::severity::trace)
        ->add_file_stream("sorted list allocator trace logs.txt", logger::severity::trace)
        ->build();
    delete builder;
    
    allocator_boundary_descriptor alc(10000, nullptr, logger, allocator_fit_allocation::allocation_mode::first_fit);

    alc.allocate(1000);
    return 0;
}