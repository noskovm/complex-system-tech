#include "allocator_boundary_descriptor.h"

allocator_boundary_descriptor::allocator_boundary_descriptor(
    size_t requested_memory_size,
    allocator* outer_allocator,
    logger* log,
    allocator_fit_allocation::allocation_mode allocation_mode) {
    
    std::string got_typename = get_typename();

    if (log != nullptr)
    {
        log->trace(got_typename + " allocator instance construction started")
            ->debug("requested memory size: " + std::to_string(requested_memory_size) + " bytes");
    }

    size_t minimal_trusted_memory_size = get_available_block_service_block_size();

    if (requested_memory_size < minimal_trusted_memory_size)
    {
        auto error_message = "trusted memory size should be GT " + std::to_string(minimal_trusted_memory_size) + " bytes";

        if (log != nullptr)
        {
            log->error(error_message);
        }

        throw allocator::memory_exception(error_message);
    }

    size_t allocator_service_block_size = get_allocator_service_block_size();

    _trusted_memory = outer_allocator == nullptr
        ? ::operator new(requested_memory_size + allocator_service_block_size)
        : outer_allocator->allocate(requested_memory_size + allocator_service_block_size);


    // устанавливаем размер доверенной области памяти
    auto * const memory_size_space = reinterpret_cast<size_t *>(_trusted_memory);
    *memory_size_space = requested_memory_size;

    // сохряняем outer allocator
    auto * const outer_allocator_pointer_space = reinterpret_cast<allocator **>(memory_size_space + 1);
    *outer_allocator_pointer_space = outer_allocator;

    // сохряняем logger
    auto * const logger_pointer_space = reinterpret_cast<logger **>(outer_allocator_pointer_space + 1);
    *logger_pointer_space = log;

    // сохряняем режим выделения памяти
    auto * const allocation_mode_space = reinterpret_cast<allocator_fit_allocation::allocation_mode *>(logger_pointer_space + 1);
    *allocation_mode_space = allocation_mode;

    // указатель на первый доступный блок
    auto * const first_available_block_pointer_space = reinterpret_cast<void **>(allocation_mode_space + 1);
    *first_available_block_pointer_space = reinterpret_cast<void *>(first_available_block_pointer_space + 1);

    std::cout << "АДРЕС ПЕРВОГО ДОСТУПНОГО \n" << *first_available_block_pointer_space;

    // left tag блока
    bool* first_available_block_left_tag_space = reinterpret_cast<bool*>(first_available_block_pointer_space + 1);
    *first_available_block_left_tag_space = true;

    // left size блока
    size_t* first_available_block_left_size_space = reinterpret_cast<size_t *>(first_available_block_left_tag_space + 1);
    *first_available_block_left_size_space = requested_memory_size;

    // указатель на следующий свободный блок
    void** first_available_block_next_block_address_space = reinterpret_cast<void **>(first_available_block_left_size_space + 1);
    *first_available_block_next_block_address_space = nullptr;

    // указатель на предыдущий свободный блок
    void** first_available_block_prev_block_address_space = first_available_block_next_block_address_space + 1;
    *first_available_block_prev_block_address_space = nullptr;

    // right size свободного блока //TODO тут плохо, мы к указателю size прибавляем
    bool* first_available_block_right_tag_space = reinterpret_cast<bool*>(reinterpret_cast<unsigned char*>(first_available_block_left_tag_space) + 
                                                                                      requested_memory_size - 
                                                                                      sizeof(size_t) - 
                                                                                      sizeof(bool));
    *first_available_block_right_tag_space = true;

    size_t* first_available_block_right_size_space = reinterpret_cast<size_t*>(first_available_block_right_tag_space + 1);
    *first_available_block_right_size_space = requested_memory_size;

    this->trace_with_guard(got_typename + " allocator instance construction finished"); // TODO разобарться почему в allocator sorted list with guard
}

void* allocator_boundary_descriptor::allocate(size_t requested_block_size){
   
    const std::string got_typename = get_typename();

    this->trace_with_guard("Method `void *" + got_typename + "::allocate(size_t requested_block_size)` execution started")
        ->debug_with_guard("Requested " + std::to_string(requested_block_size) + " bytes of memory");
    

    //TODO понять зачем
    size_t requested_block_size_overridden = requested_block_size;
    if (requested_block_size_overridden < sizeof(void *)) {
         requested_block_size_overridden = sizeof(void *);
    }

    void* previous_block = nullptr;
    void* current_block = get_first_available_block_address();

    void* target_block = nullptr;
    void* previous_to_target_block = nullptr;
    void* next_to_target_block = nullptr;

    // сколько байт нужно для хранения системной информации о блоке
    auto const available_block_service_block_size = get_available_block_service_block_size();

    // сколько байт нужно для хранения сис.инфы занятого блока
    auto const occupied_block_service_block_size = get_occupied_block_service_block_size();

    auto const allocation_mode = get_allocation_mode();

    while (current_block != nullptr)
    {
        // получаем размер текущего блока и ссылку на следующий
        auto const current_block_size = get_available_block_size(current_block);
        auto const next_block = get_available_block_next_available_block_address(current_block);

        // если рамзер текущего блока больше чем запрашивает пользователь + размер для хранения системной инфы блока
        if (current_block_size >= requested_block_size_overridden + occupied_block_service_block_size)
        {
            if (allocation_mode == allocator_fit_allocation::allocation_mode::first_fit ||
                allocation_mode == allocator_fit_allocation::allocation_mode::the_best_fit && (target_block == nullptr || current_block_size < get_available_block_size(target_block)) ||
                allocation_mode == allocator_fit_allocation::allocation_mode::the_worst_fit && (target_block == nullptr || current_block_size > get_available_block_size(target_block)))
            {
                previous_to_target_block = previous_block;
                target_block = current_block;
                next_to_target_block = next_block;
            }

            if (allocation_mode == allocator_fit_allocation::allocation_mode::first_fit)
            {
                break;
            }
        }

        previous_block = current_block;
        current_block = next_block;
    }

    // если дошли до конца, значит не нашли подходящего блока
    if (target_block == nullptr)
    {
        auto const warning_message = "no memory available to allocate";

        this->warning_with_guard(warning_message)
            ->trace_with_guard("Method `void *" + got_typename + "::allocate(size_t requested_block_size)` execution finished");

        throw memory_exception(warning_message);
    }


    auto const target_block_size = get_available_block_size(target_block);

    // если оставшийся хвостик, его размера не достаточно для хранения свободного блока, то отдаем весь блок
    if (target_block_size - requested_block_size_overridden - occupied_block_service_block_size < available_block_service_block_size)
    {
        requested_block_size_overridden = target_block_size - occupied_block_service_block_size;
    }

    // обновляем запрашиваемый размер
    if (requested_block_size_overridden != requested_block_size)
    {
        this->trace_with_guard("Requested " + std::to_string(requested_block_size) + " bytes, but reserved " + std::to_string(requested_block_size_overridden) + " bytes in according to correct work of allocator");

        requested_block_size = requested_block_size_overridden;
    }

    void* updated_next_block_to_previous_block;

    // если забираем весь блок без остатка, то нужно перебросить указатель next и prev
    if (requested_block_size == target_block_size - occupied_block_service_block_size)
    {

        if (previous_to_target_block != nullptr)
            *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(previous_to_target_block) + sizeof(bool) + sizeof(size_t)) = next_to_target_block;

        if (next_to_target_block != nullptr)
            *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(next_to_target_block) + sizeof(size_t) + sizeof(bool) + sizeof(void*)) = previous_to_target_block;

        updated_next_block_to_previous_block = next_to_target_block;
    }
    else
    {
        // если забираем часть блока, то обнволем указатли prev и next на новый свободный блок
        updated_next_block_to_previous_block = reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(target_block) + occupied_block_service_block_size + requested_block_size);

        // запись информации в оставшийся свободный блок
        // TODO поменять местами тэг и size
        auto* const target_block_leftover_size = reinterpret_cast<size_t*>(updated_next_block_to_previous_block);
        *target_block_leftover_size = target_block_size - occupied_block_service_block_size - requested_block_size;

        auto* const target_block_leftover_tag = reinterpret_cast<bool*>(target_block_leftover_size + 1);
        *target_block_leftover_tag = true;
        //

        // устанавливаем указатели на новый предыдущий и следующий свободный блок
        auto* const target_block_leftover_next_block_address = reinterpret_cast<void**>(target_block_leftover_tag + 1);
        *target_block_leftover_next_block_address = next_to_target_block;

        auto* const target_block_leftover_prev_block_address_space = reinterpret_cast<void**>(target_block_leftover_next_block_address + 1);
        *target_block_leftover_prev_block_address_space = previous_to_target_block;

        auto* const target_block_leftover_footer_tag = reinterpret_cast<bool*>(reinterpret_cast<unsigned char*>(updated_next_block_to_previous_block) + *target_block_leftover_size - sizeof(bool) - sizeof(size_t));
        *target_block_leftover_footer_tag = true;

        auto* const target_block_leftover_footer_size = reinterpret_cast<size_t*>(reinterpret_cast<unsigned char*>(updated_next_block_to_previous_block) + *target_block_leftover_size - sizeof(size_t));
        *target_block_leftover_footer_size = target_block_size - occupied_block_service_block_size - requested_block_size;

        // обноляем previous_to_target_block и next_to_target_block
        if (previous_to_target_block != nullptr)
            *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(previous_to_target_block) + sizeof(size_t) + sizeof(bool)) = updated_next_block_to_previous_block;
        if (next_to_target_block != nullptr)
            *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(next_to_target_block) + sizeof(size_t) + sizeof(bool) + sizeof(void*)) = updated_next_block_to_previous_block;
    }

    // обновляем адрес первого свободного блока
    if (previous_to_target_block == nullptr)
        *get_first_available_block_address_address() = updated_next_block_to_previous_block;
    else
        *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(previous_to_target_block) + sizeof(size_t) + sizeof(bool)) = updated_next_block_to_previous_block;

    // обновляем инфу о занятом блоке
    auto* target_block_size_address = reinterpret_cast<size_t*>(target_block);
    *target_block_size_address = requested_block_size + 2 * sizeof(size_t) + 2 * sizeof(bool);

    auto* const target_block_is_free_space = reinterpret_cast<bool*>(reinterpret_cast<size_t*>(target_block) + 1);
    *target_block_is_free_space = false;

    auto* const target_block_footer_space = reinterpret_cast<bool*>(reinterpret_cast<unsigned char*>(target_block) + *target_block_size_address - sizeof(bool) - sizeof(size_t));
    *target_block_footer_space = false;

    auto* target_block_footer_size_address = reinterpret_cast<size_t*>(reinterpret_cast<unsigned char*>(target_block) + *target_block_size_address - sizeof(size_t));
    *target_block_footer_size_address = requested_block_size + 2 * sizeof(size_t) + 2 * sizeof(bool);

    auto* const allocated_block = reinterpret_cast<void*>(target_block_is_free_space + 1);

    this->trace_with_guard("Allocated block placed at " + address_to_hex(allocated_block))
        ->trace_with_guard("Method `void *" + got_typename + "::allocate(size_t requested_block_size)` execution finished");

    this->debug_with_guard("After `allocate` for " + std::to_string(requested_block_size) + " bytes (addr == " +
        address_to_hex(target_block_size_address) + "):");
    dump_trusted_memory_blocks_state();
    return allocated_block;
};

void allocator_boundary_descriptor::deallocate(void* target_to_dealloc){
    // TODO реализовать
    return void(0);
};

void* allocator_boundary_descriptor::reallocate(void* target_to_realloc, size_t new_size){
    return nullptr;
};

size_t allocator_boundary_descriptor::get_available_block_size(void *block) {
    return *reinterpret_cast<size_t*>(reinterpret_cast<bool*>(block) + 1); // TODO проверить
}

void* allocator_boundary_descriptor::get_next_link(void *block){
    std::cout << "Адрес ссылки на следуюший блок блок \n" << 
    reinterpret_cast<void*>(reinterpret_cast<size_t*>(reinterpret_cast<bool*>(block) + 1) + 1);
    return reinterpret_cast<void*>(reinterpret_cast<size_t*>(reinterpret_cast<bool*>(block) + 1) + 1); // TODO проверить
}

void* allocator_boundary_descriptor::get_prev_link(void *block){
    std::cout << "Адрес ссылки на преыдущий блок \n" << 
    (reinterpret_cast<void*>(reinterpret_cast<size_t*>(reinterpret_cast<bool*>(block) + 1) + 1) + 1);
    return (reinterpret_cast<void*>(reinterpret_cast<size_t*>(reinterpret_cast<bool*>(block) + 1) + 1) + 1); // TODO проверить
}

void allocator_boundary_descriptor::set_block_size(void *block, size_t new_size){
    std::cout << "Текущий размер блока \n" << 
    *reinterpret_cast<size_t*>(reinterpret_cast<bool*>(block) + 1);
    *reinterpret_cast<size_t*>(reinterpret_cast<bool*>(block) + 1) = new_size;  // TODO проверить
}

std::string allocator_boundary_descriptor::get_typename() {
    return "allocator boundary descriptor";
};

size_t allocator_boundary_descriptor::get_available_block_service_block_size() {
    size_t tag_size  = sizeof(bool);
    size_t current_block_size = sizeof(size_t);
    size_t available_block_pointer_size = sizeof(void *);

    return 2*tag_size + 2*current_block_size + 2*available_block_pointer_size;
}

size_t allocator_boundary_descriptor::get_allocator_service_block_size() {
    size_t trusted_memory_size = sizeof(size_t);
    size_t outer_allocator_pointer_size = sizeof(allocator *);
    size_t logger_pointer_size = sizeof(logger *);
    size_t allocation_mode_size = sizeof(allocator_fit_allocation::allocation_mode);
    size_t first_available_block_pointer_size = sizeof(void *);

    return trusted_memory_size + outer_allocator_pointer_size + logger_pointer_size + allocation_mode_size + first_available_block_pointer_size;
}

// получить указатель на указатель первый свободный блок памяти (void**)
void** allocator_boundary_descriptor::get_first_available_block_address_address() {
    return reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(size_t) + sizeof(allocator *) + sizeof(logger *) + sizeof(allocator_fit_allocation::allocation_mode));
}

// получить указатель первый свободный блок (void*)
void* allocator_boundary_descriptor::get_first_available_block_address() {
    return *get_first_available_block_address_address();
}

logger* allocator_boundary_descriptor::get_logger() const noexcept {
    return *reinterpret_cast<logger **>(reinterpret_cast<allocator **>(reinterpret_cast<size_t *>(_trusted_memory) + 1) + 1);
}

size_t allocator_boundary_descriptor::get_occupied_block_service_block_size() {
    return sizeof(bool)*2 + sizeof(size_t);
}

allocator_fit_allocation::allocation_mode allocator_boundary_descriptor::get_allocation_mode() {
    return *reinterpret_cast<allocator_fit_allocation::allocation_mode *>(reinterpret_cast<unsigned char *>(_trusted_memory) + sizeof(size_t) + sizeof(allocator *) + sizeof(logger *));
}

void* allocator_boundary_descriptor::get_available_block_next_available_block_address(void* current_block_address) {
    return  *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(current_block_address) + sizeof(bool) + sizeof(size_t));
}

std::string allocator_boundary_descriptor::address_to_hex(void const * const pointer) noexcept {
    return std::string { (std::stringstream() << pointer).str() };
}

void allocator_boundary_descriptor::dump_trusted_memory_blocks_state() {
    if (get_logger() == nullptr)
    {
        return;
    }

    std::string to_dump("|");
    auto memory_size = get_trusted_memory_size();
    auto current_available_block = get_first_available_block_address();
    unsigned char *first_block = reinterpret_cast<unsigned char *>(_trusted_memory) + get_allocator_service_block_size();
    unsigned char *current_block = first_block;

    while (current_block - first_block < memory_size)
    {
        size_t current_block_size;
        if (current_block == current_available_block)
        {
            current_block_size = get_available_block_size(current_block);
            to_dump += "avl ";
            current_available_block = get_available_block_next_available_block_address(current_available_block);
        }
        else
        {
            current_block_size = get_occupied_block_size(current_block);
            to_dump += "occ ";
        }

        to_dump += std::to_string(current_block_size) + "|";
        current_block += current_block_size;
    }

    this->debug_with_guard("Memory state: " + to_dump);
}

size_t allocator_boundary_descriptor::get_trusted_memory_size() {
    return *reinterpret_cast<size_t *>(_trusted_memory);
}

size_t allocator_boundary_descriptor::get_occupied_block_size(void const *current_block_address) const {
    return *reinterpret_cast<size_t const *>(current_block_address);
}

allocator* allocator_boundary_descriptor::get_allocator() const noexcept {
    return *reinterpret_cast<allocator **>(reinterpret_cast<size_t *>(_trusted_memory) + 1);
}


bool allocator_boundary_descriptor::reallocate(void** block_to_reallocate_address_address, size_t new_block_size) {
    // TODO ?
    return false;
}