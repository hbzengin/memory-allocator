#ifndef ALLOCATOR
#define ALLOCATOR

#include <stddef.h> // size_t

void *malloc(size_t size);
void free(void *ptr);
void *realloc(void *ptr, size_t new_size);
void *calloc(size_t nmemb, size_t size);

#endif