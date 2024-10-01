#include "memory_manager.h"
#include <list>
#include <unistd.h>  // for sbrk()
#include <iostream>

extern std::list<Allocation> allocatedList;
extern std::list<Allocation> freeList;

// First Fit allocation logic
void* firstFitAlloc(std::size_t chunk_size) {
    for (auto it = freeList.begin(); it != freeList.end(); ++it) {
        if (it->size >= chunk_size) {
            Allocation newAlloc = *it;
            freeList.erase(it);
            allocatedList.push_back(newAlloc);
            return newAlloc.space;
        }
    }

    // If no chunk found, request new memory from OS
    void* newSpace = sbrk(chunk_size);
    if (newSpace == (void*)-1) {
        std::cerr << "Memory allocation failed!" << std::endl;
        return nullptr;
    }

    Allocation newAlloc = { chunk_size, newSpace };
    allocatedList.push_back(newAlloc);
    return newSpace;
}

// Deallocation logic
void firstFitDealloc(void* chunk) {
    for (auto it = allocatedList.begin(); it != allocatedList.end(); ++it) {
        if (it->space == chunk) {
            Allocation freeChunk = *it;
            allocatedList.erase(it);
            freeList.push_back(freeChunk);
            return;
        }
    }
    std::cerr << "Error: Attempting to free unallocated memory!" << std::endl;
    std::terminate();
}
