#ifndef FIRST_FIT_H
#define FIRST_FIT_H

#include <cstddef> // for std::size_t

// Function declarations
void* firstFitAlloc(std::size_t chunk_size);
void firstFitDealloc(void* chunk);

#endif // FIRST_FIT_H
