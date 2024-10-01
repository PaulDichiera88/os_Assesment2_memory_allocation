#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <cstddef> // for std::size_t

struct Allocation {
    std::size_t size;
    void* space;

    // Define the equality operator for Allocation
    bool operator==(const Allocation& other) const {
        return size == other.size && space == other.space;
    }
};


// allocation and deallocation functions declaration
void* alloc(std::size_t chunk_size);
void dealloc(void* chunk);

// set the allocation strategy function, implementation of these functions will cvhnage based on the startergy
void setAllocationStrategy(void* (*alloc_func)(std::size_t), void (*dealloc_func)(void*)); // allows switching between allocation logic without any changes to the program logic

// Global variables for allocation and deallocation function pointers
extern void* (*current_alloc)(std::size_t);
extern void (*current_dealloc)(void*);

#endif // MEMORY_MANAGER_H
