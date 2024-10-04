#include "best_fit.h"
#include <list>
#include <unistd.h>  // for sbrk()
#include <iostream>

// Global lists for allocated and free memory chunks
std::list<Allocation> allocatedList;
std::list<Allocation> freeList;
const std::size_t PARTITION_SIZES[] = {32, 64, 128, 256, 512};


// Best Fit allocation function
void* bestFitAlloc(std::size_t requested_size) {
    std::size_t total_size = findPartitionSize(requested_size);

    auto bestFit = freeList.end();
    for (auto it = freeList.begin(); it != freeList.end(); ++it) {
        if (it->total_size >= requested_size && (bestFit == freeList.end() || it->total_size < bestFit->total_size)) {
            bestFit = it;
        }
    }

    if (bestFit != freeList.end()) {
        Allocation newAlloc = *bestFit;
        freeList.erase(bestFit);
        allocatedList.push_back(newAlloc);
        return newAlloc.space;
    }

    void* newSpace = sbrk(total_size);
    if (newSpace == (void*)-1) {
        std::cerr << "Memory allocation failed!" << std::endl;
        return nullptr;
    }

    Allocation newAlloc = { requested_size, total_size, newSpace };
    allocatedList.push_back(newAlloc);
    return newSpace;
}

// Best Fit deallocation function
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

std::size_t findPartitionSize(std::size_t requested_size) {
    for (std::size_t partition : PARTITION_SIZES) {
        if (requested_size <= partition) {
            return partition;
        }
    }
    // If requested_size exceeds all defined partition sizes, return the largest size
    return PARTITION_SIZES[sizeof(PARTITION_SIZES) / sizeof(PARTITION_SIZES[0]) - 1];
}

