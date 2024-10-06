#include "best_fit.h"
#include <list>
#include <unistd.h>  // for sbrk()
#include <iostream>

// Global lists for allocated and free memory chunks (using pointers)
std::list<Allocation*> allocatedList;
std::list<Allocation*> freeList;
const std::size_t PARTITION_SIZES[] = {32, 64, 128, 256, 512};

// Best Fit allocation function
void* alloc(std::size_t chunk_size) {
    Allocation* selected_block = bestFitSearch(chunk_size);

    if (selected_block != nullptr) {

        void* allocated_space = selected_block->space;

        if (allocated_space != nullptr) {
            // Now safely move the pointer to the allocated list
            selected_block->requested_size = chunk_size;  // Reflect the requested size even though total_size is larger
            allocatedList.push_back(selected_block);

            // Remove the block from the free list after confirming it's valid
            auto it = std::find(freeList.begin(), freeList.end(), selected_block);
            if (it != freeList.end()) {
                freeList.erase(it);
            }
            
            return allocated_space;  // Return the space pointer
        } else {
            std::cerr << "Error: Unable to allocate memory. Space pointer is null." << std::endl;
            return nullptr;
        }
    }

    // If no block was found in the free list, allocate new memory
    std::size_t total_size = findPartitionSize(chunk_size);
    void* new_memory = sbrk(total_size);

    if (new_memory == (void*)-1) {
        std::cerr << "Memory allocation failed!" << std::endl;
        return nullptr;
    }

    Allocation* newAlloc = new Allocation{chunk_size, total_size, new_memory};
    allocatedList.push_back(newAlloc);
    return new_memory;
}

// Best Fit deallocation function
void dealloc(void* chunk) {
    for (auto it = allocatedList.begin(); it != allocatedList.end(); ++it) {
        if ((*it)->space == chunk) {
            Allocation* freeChunk = *it;
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
    return PARTITION_SIZES[sizeof(PARTITION_SIZES) / sizeof(PARTITION_SIZES[0]) - 1];
}

Allocation* bestFitSearch(std::size_t chunk_size) {
    Allocation* best = nullptr;
    std::size_t smallest_difference = std::numeric_limits<std::size_t>::max();

    for (auto& block : freeList) {
        if (block->total_size >= chunk_size) {
            std::size_t difference = block->total_size - chunk_size;
            if (difference < smallest_difference) {
                smallest_difference = difference;
                best = block;
            }
        }
    }
    
    return best;
}
