#include "memory_manager.h"
#include <list>
#include <unistd.h>  // for sbrk()
#include <iostream>

extern std::list<Allocation> allocatedList;
extern std::list<Allocation> freeList;

// Best Fit allocation logic
void* bestFitAlloc(std::size_t chunk_size) {
    Allocation* bestFit = nullptr;
    for (auto& chunk : freeList) {
        if (chunk.size >= chunk_size) {
            if (!bestFit || chunk.size < bestFit->size) {
                bestFit = &chunk;
            }
        }
    }

    if (bestFit) {
        Allocation newAlloc = *bestFit;
        freeList.remove(*bestFit);
        allocatedList.push_back(newAlloc);
        return newAlloc.space;
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
void bestFitDealloc(void* chunk) {
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
