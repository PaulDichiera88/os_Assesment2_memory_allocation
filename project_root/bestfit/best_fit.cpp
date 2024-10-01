#include <list>
#include <unistd.h>  // for sbrk()
#include <iostream>

// Allocation struct
struct Allocation {
    std::size_t size;
    void* space;

    // Define the equality operator for Allocation
    bool operator==(const Allocation& other) const {
        return size == other.size && space == other.space;
    }
};

// Global lists for allocated and free memory chunks
std::list<Allocation> allocatedList;
std::list<Allocation> freeList;

// Best Fit allocation function
void* bestFitAlloc(std::size_t chunk_size) {
    // Best Fit Allocation logic
    auto bestFit = freeList.end();
    for (auto it = freeList.begin(); it != freeList.end(); ++it) {
        if (it->size >= chunk_size && (bestFit == freeList.end() || it->size < bestFit->size)) {
            bestFit = it;
        }
    }

    if (bestFit != freeList.end()) {
        Allocation newAlloc = *bestFit;
        freeList.erase(bestFit);
        allocatedList.push_back(newAlloc);
        return newAlloc.space;
    }

    // If no chunk is found, request more memory from OS
    void* newSpace = sbrk(chunk_size);
    if (newSpace == (void*)-1) {
        std::cerr << "Memory allocation failed!" << std::endl;
        return nullptr;
    }

    Allocation newAlloc = { chunk_size, newSpace };
    allocatedList.push_back(newAlloc);
    return newSpace;
}

// Best Fit deallocation function
void bestFitDealloc(void* chunk) {
    // Best Fit Deallocation logic
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
