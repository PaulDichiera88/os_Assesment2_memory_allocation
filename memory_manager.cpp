#include "memory_manager.h"
#include <list>

// Lists for allocated and free memory chunks
std::list<Allocation> allocatedList;
std::list<Allocation> freeList;

// Pointers to the current allocation and deallocation functions
void* (*current_alloc)(std::size_t) = nullptr;
void (*current_dealloc)(void*) = nullptr;

// Function to set the current strategy
void setAllocationStrategy(void* (*alloc_func)(std::size_t), void (*dealloc_func)(void*)) {
    current_alloc = alloc_func;
    current_dealloc = dealloc_func;
}

// Wrapper for calling the current allocation function
void* alloc(std::size_t chunk_size) {
    if (current_alloc) {
        return current_alloc(chunk_size);
    }
    return nullptr;
}

// Wrapper for calling the current deallocation function
void dealloc(void* chunk) {
    if (current_dealloc) {
        current_dealloc(chunk);
    }
}
