#ifndef FIRST_FIT_H
#define FIRST_FIT_H

#include <cstddef> 
#include <list>    

// allocation structure
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

// global lists
extern std::list<Allocation*> allocatedList;  
extern std::list<Allocation*> freeList;       

// function declarations
void* alloc(std::size_t chunk_size);
Allocation* firstFitSearch(std::size_t chunk_size);
void dealloc(void* chunk);
std::size_t findPartitionSize(std::size_t requested_size);

#endif // FIRST_FIT_H
