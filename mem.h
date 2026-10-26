/*
    Compressor memory manager
*/
#ifndef MEM_H
#define MEM_H

#include <stdint.h>

#define INITAL_LIST_SIZE 25

// Structure for memory allocated
typedef struct {
    void* ptr;
    size_t size;
    int active;
} AllocatedMemory;

// Safely allocate memory that will be tracked
void* s_malloc(size_t size);

// Safely reallocate memory...
void* s_realloc(void* ptr, size_t newSize);

// Free some memory
void s_free(void* location);

// Track some memory
void addAllocatedMemory(AllocatedMemory mem);

// Done
void removeAllocatedMemory(void* location);

// Quit
void memQuit();

#endif