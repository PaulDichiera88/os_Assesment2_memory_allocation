#ifndef FIRST_FIT_H
#define FIRST_FIT_H

#include <cstddef> // for std::size_t
#include <list>    // for std::list

// Allocation structure
struct Allocation {
    std::size_t size;
    void* space;

    bool operator==(const Allocation& other) const {
        return size == other.size && space == other.space;
    }
};

// Expose the global lists as extern
extern std::list<Allocation> allocatedList;
extern std::list<Allocation> freeList;

// Function declarations
void* firstFitAlloc(std::size_t chunk_size);
void firstFitDealloc(void* chunk);

#endif // FIRST_FIT_H
