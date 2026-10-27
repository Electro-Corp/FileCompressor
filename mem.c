/*
    Compressor memory manager
*/
#include <stdlib.h>
#include <stdio.h>
#include "mem.h"

// Tracking for memory
static size_t totalMemoryAllocated = 0;
// Array of all allocated memory
static AllocatedMemory** currentlyAllocatedMemory;
static int firstAvaliableMemory = 0, allocationListSize = 0;

// Safely allocate memory that will be tracked
void* s_malloc(size_t size){
    // printf("======= MEMORY ALLOCATION REQUEST ======:\n");
    // printf("Total memory allocated: %d, Allocation List Size: %d, First Block Ready: %d\n", totalMemoryAllocated, allocationListSize, firstAvaliableMemory);
    // printf("Just requested memory: %d\n", size);
    // Allocate the memory through the kernel
    void* memory = malloc(size);
    // Track it
    AllocatedMemory* mem = (AllocatedMemory*)malloc(sizeof(AllocatedMemory));
    mem->active = 1;
    mem->ptr = memory;
    mem->size = size;
    addAllocatedMemory(mem);
    totalMemoryAllocated += size;
    // Return
    return memory;
}

// Safely reallocate memory...
void* s_realloc(void* ptr, size_t newSize){
    // printf("======= MEMORY REALLOCATION REQUEST ======:\n");
    // printf("Total memory allocated: %d, Allocation List Size: %d, First Block Ready: %d\n", totalMemoryAllocated, allocationListSize, firstAvaliableMemory);
    // printf("Just requested memory: %d\n", newSize);
    ptr = realloc(ptr, newSize);
    if(ptr == NULL){
        printf("Realloc failed: System out of memory!\n");
        exit(-1);
    }
    // Update internally
    for(int i = 0; i < allocationListSize; i++){
        if(currentlyAllocatedMemory[i] != NULL){
            if(currentlyAllocatedMemory[i]->ptr == ptr){
                totalMemoryAllocated += newSize - currentlyAllocatedMemory[i]->size;
                currentlyAllocatedMemory[i]->size = newSize;
            }
        }
    }
    return ptr;
}

// Free some memory
void s_free(void* location){
    free(location);
    removeAllocatedMemory(location);
}

// Track some memory
void addAllocatedMemory(AllocatedMemory* mem){
    // Do we need to start tracking?
    if(currentlyAllocatedMemory == NULL){
        currentlyAllocatedMemory = calloc(INITAL_LIST_SIZE, sizeof(AllocatedMemory));
        allocationListSize = INITAL_LIST_SIZE;
    }
    // Check if we have a slot ready
    if(firstAvaliableMemory < allocationListSize){
        // If we do have a slot, allocate it
        currentlyAllocatedMemory[firstAvaliableMemory] = mem;
        // Find the next avaliable slot for next time
        int found = 0;
        for(int i = 0; i < allocationListSize; i++){
            if(currentlyAllocatedMemory[i] != NULL){
                if(!currentlyAllocatedMemory[i]->active){
                    firstAvaliableMemory = i;
                    found = 1;
                    break;
                }
            }else{
                firstAvaliableMemory = i;
                found = 1;
                break;
            }
        }
        if(!found){
            currentlyAllocatedMemory = realloc(currentlyAllocatedMemory, sizeof(AllocatedMemory) * (allocationListSize + 5));
            // Check if realloc failed
            if(currentlyAllocatedMemory == NULL){
                printf("Realloc failed: System out of memory!\n");
                exit(-1);
            }
            // Clear out new memory
            for(int i = allocationListSize; i < allocationListSize + 5; i++) currentlyAllocatedMemory[i] = NULL;
            // Copy over new
            currentlyAllocatedMemory[allocationListSize + 1] = mem;
            // Set vars
            firstAvaliableMemory = allocationListSize + 2;
            allocationListSize += 5;
        }
    }else{
            currentlyAllocatedMemory = realloc(currentlyAllocatedMemory, sizeof(AllocatedMemory) * (allocationListSize + 5));
            // Check if realloc failed
            if(currentlyAllocatedMemory == NULL){
                printf("Realloc failed: System out of memory!\n");
                exit(-1);
            }
            // Clear out new memory
            for(int i = allocationListSize; i < allocationListSize + 5; i++) currentlyAllocatedMemory[i] = NULL;
            // Copy over new
            currentlyAllocatedMemory[allocationListSize + 1] = mem;
            // Set vars
            firstAvaliableMemory = allocationListSize + 2;
            allocationListSize += 5;
    }
}

// Done
void removeAllocatedMemory(void* location){
    for(int i = 0; i < allocationListSize; i++){
        if(currentlyAllocatedMemory[i] != NULL){
            if(currentlyAllocatedMemory[i]->ptr == location){
                totalMemoryAllocated -= currentlyAllocatedMemory[i]->size;
                if(firstAvaliableMemory > i) firstAvaliableMemory = i;
                currentlyAllocatedMemory[i]->active = 0;
                break;
            }
        }   
    }
}

size_t getTotalMemoryUsed(){
    return totalMemoryAllocated;
}

// Debug info
void printMmeoryDebugInfo(){
    printf("======= MEMORY INFO ======:\n");
    printf("Total memory allocated: %lu, Allocation List Size: %d, First Block Ready: %d\n", totalMemoryAllocated, allocationListSize, firstAvaliableMemory);
}

// Clear all memory
void memQuit(){
    for(int i = 0; i < allocationListSize; i++){
        if(currentlyAllocatedMemory[i]->active){
            free(currentlyAllocatedMemory[i]->ptr);
            free(currentlyAllocatedMemory[i]);   
        }
    }
    free(currentlyAllocatedMemory);
}