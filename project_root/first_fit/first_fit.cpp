#include "first_fit.h"
#include <list>
#include <unistd.h>  // for sbrk()
#include <iostream>

// Global lists for allocated and free memory chunks
std::list<Allocation> allocatedList;
std::list<Allocation> freeList;
const std::size_t PARTITION_SIZES[] = {32, 64, 128, 256, 512};

// First Fit allocation function
void* alloc(std::size_t chunk_size) {
    Allocation* selected_block = firstFitSearch(chunk_size);

        if (selected_block != nullptr) {
        std::cout << "Best fit found: " << selected_block->total_size << " bytes, at address: " << selected_block->space << std::endl;

        // Move the selected block to allocatedList but only after ensuring it's valid
        void* allocated_space = selected_block->space;  // Save the space pointer first
        if (allocated_space != nullptr) {
            // Now safely move the block to the allocated list
            allocatedList.push_back(*selected_block);

            // Remove the block from the free list after confirming it's valid
            auto it = std::find(freeList.begin(), freeList.end(), *selected_block);  // Find the element in the list
            if (it != freeList.end()) {
                std::cout << "Removing block: " << selected_block->space << " from free list" << std::endl;
                freeList.erase(it);  // Erase the specific element from the freeList
            }

            std::cout << "Allocating block: " << selected_block->space << std::endl;
            return allocated_space;  // Return the space pointer
        } else {
            std::cerr << "Error: Unable to allocate memory. Space pointer is null." << std::endl;
            return nullptr;
        }
    }

    std::size_t total_size = findPartitionSize(chunk_size);

    void* new_memory = sbrk(total_size);

    if (new_memory == (void*)-1) {
        std::cerr << "Memory allocation failed!" << std::endl;
        return nullptr;
    }

    Allocation newAlloc = {chunk_size, total_size, new_memory };
    allocatedList.push_back(newAlloc);
    return new_memory;
}

// First Fit deallocation function
void dealloc(void* chunk) {
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

Allocation* firstFitSearch(std::size_t chunk_size) {
    for (auto& block : freeList) {
        if (block.total_size >= chunk_size) {
            return &block;
        }
    }
    return nullptr;
}
