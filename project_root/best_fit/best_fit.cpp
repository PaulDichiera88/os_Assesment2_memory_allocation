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
        std::cout << "Best fit found: " << selected_block->total_size << " bytes, at address: " << selected_block->space << std::endl;

        void* allocated_space = selected_block->space;

        if (allocated_space != nullptr) {
            // Now safely move the pointer to the allocated list
            selected_block->requested_size = chunk_size;  // Reflect the requested size even though total_size is larger
            allocatedList.push_back(selected_block);

            // Remove the block from the free list after confirming it's valid
            auto it = std::find(freeList.begin(), freeList.end(), selected_block);
            if (it != freeList.end()) {
                std::cout << "Removing block from free list: " << selected_block->space << std::endl;
                freeList.erase(it);
            }
            std::cout << "Allocating block: " << selected_block->space << std::endl;
            std::cout << "Block of size " << chunk_size << " allocated at address: " << selected_block->space << std::endl;
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
            std::cout << "Pushed back to freeList: address " << freeChunk->space << ", size: " << freeChunk->total_size << std::endl;
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

// Search for best fit
Allocation* bestFitSearch(std::size_t chunk_size) {
    Allocation* best = nullptr;
    for (auto& block : freeList) {
        if (block->total_size >= chunk_size && (!best || block->total_size < best->total_size)) {
            best = block;
        }
    }
    if (best) {
        std::cout << "Best fit found: " << best->total_size << " bytes, at address: " << best->space << std::endl;
    }
    return best;
}
