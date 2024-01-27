#include <iostream>

#include "problem_set_1/allocator.h"
#include "../3rdParty/logger/logger_builder.h"
#include "../3rdParty/logger/logger_builder_concrete.h"

int main() {
    logger_builder *builder = new logger_builder_concrete();
    auto *logger = builder
        ->add_console_stream(logger::severity::debug)
        ->add_file_stream("sorted list allocator trace logs.txt", logger::severity::trace)
        ->build();
    delete builder;

    std::cout << "Hello World" << std::endl;

    simple_allocator smpl_allocator;

    void* allocated_block = smpl_allocator.allocate(1000);

    return 0;
}