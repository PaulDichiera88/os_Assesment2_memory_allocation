#ifndef BEST_FIT_H
#define BEST_FIT_H

#include <cstddef> // for std::size_t
#include <list>    // for std::list

// Allocation structure
struct Allocation {
    std::size_t requested_size;  // size requested by the user
    std::size_t total_size;      // size of the allocation based on partition size
    void* space;                 // address of memory chunk

bool operator==(const Allocation& other) const {
    return requested_size == other.requested_size 
        && total_size == other.total_size 
        && space == other.space;
    }
};

// Expose the global lists as extern
extern std::list<Allocation> allocatedList;
extern std::list<Allocation> freeList;

// Function declarations
void* bestFitAlloc(std::size_t chunk_size);
void bestFitDealloc(void* chunk);
std::size_t findPartitionSize(std::size_t requested_size);

#endif // BEST_FIT_H
