#include "malloc.h"
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

/*
 * TODO:
 *-Round stuff to multiples of 8 for better alignment
 *-Split large free blocks for less internal fragmentation after free
 *-Potentially merge adjacent free blocks for reduced external fragmentation
 *-If 'tail' is unused, actually shrink the heap
 *-Instead of picking first free, pick 'best free' for less external fragm.
 */

struct header {
  size_t size;
  struct header *next;
  bool free;
  int debug;
};

#define HEADER_SIZE sizeof(struct header)

void *head = NULL;
struct header *tail = NULL;

struct header *find_free_block(size_t size) {
  struct header *current = head;
  while (current && (!current->free || current->size < size)) {
    current = current->next;
  }
  return current;
}

void *allocate_new_block(size_t size) {
  struct header *hd = sbrk(0);
  void *req = sbrk(size + HEADER_SIZE);
  assert((void *)hd == req);

  // Failure as defined on the man pages
  if (req == (void *)-1) {
    return NULL;
  }

  if (tail) {
    tail->next = req;
  }
  hd->size = size;
  hd->next = NULL;
  hd->free = false;
  tail = hd;

  return hd;
}

void *malloc(size_t size) {
  if (size <= 0) {
    return NULL;
  }

  struct header *block;
  if (!head) {
    block = allocate_new_block(size);
    if (!block) {
      return NULL;
    }
    head = block;
  } else {
    block = find_free_block(size);
    if (!block) {
      block = allocate_new_block(size);
      if (!block) {
        return NULL;
      }
    } else {
      // reusing previously used blcok
      block->free = false;
    }
  }

  return block + 1; // move exactly one unit right
}

struct header *get_header(void *data_ptr) {
  return (struct header *)data_ptr - 1;
}

void free(void *data_ptr) {
  if (!data_ptr) {
    return;
  }
  struct header *hd = get_header(data_ptr);
  hd->free = true;
}

void *realloc(void *data_ptr, size_t new_size) {
  // lack of first arg makes this behave like malloc
  if (!data_ptr) {
    return malloc(new_size);
  }

  struct header *hd = get_header(data_ptr);
  if (hd->size >= new_size) {
    return data_ptr; // reuse old space, big enough
  }

  // else, need other bigger place
  void *new_block = malloc(new_size);
  if (!new_block) {
    return NULL;
  }

  memcpy(new_block, data_ptr, hd->size);
  free(data_ptr);
  return new_block;
}

void *calloc(size_t num_elements, size_t size_of_element) {
  size_t size = num_elements * size_of_element;
  void *data_ptr = malloc(size);
  memset(data_ptr, 0, size);
  return data_ptr;
}
