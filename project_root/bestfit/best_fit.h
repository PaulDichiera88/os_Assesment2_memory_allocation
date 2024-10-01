#ifndef BEST_FIT_H
#define BEST_FIT_H

#include <cstddef> // for std::size_t

// Function declarations
void* bestFitAlloc(std::size_t chunk_size);
void bestFitDealloc(void* chunk);

#endif // BEST_FIT_H
