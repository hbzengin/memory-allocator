#include "allocator.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main() {
  puts("Starting allocator tests:");

  // 1: allocate deallocate
  {
    char *a = malloc(64);
    char *b = malloc(64);
    assert(a && b);
    free(a);
    free(b);

    char *c = malloc(32);
    assert(c == a);
    printf("    1: (c == a) OK\n");

    /* 1b) now the *next* free block (b) should be used */
    char *d = malloc(48);
    assert(d == b);
    printf("    1: (d == b) OK \n");

    free(c);
    free(d);
  }

  // 2: behavior after free()
  {
    void *blocks[5];
    for (int i = 0; i < 5; i++) {
      blocks[i] = malloc(20 + i * 10);
      assert(blocks[i]);
    }

    // free two
    free(blocks[1]);
    free(blocks[3]);

    // must pick the first block
    void *fit = malloc(15);
    assert(fit == blocks[1]);
    printf("    2: malloc, free, reuse OK\n");

    // clean
    free(fit);
    free(blocks[0]);
    free(blocks[2]);
    free(blocks[4]);
  }

  // 3: read and write to actual location
  {
    char *p = malloc(16);
    assert(p != NULL);
    strcpy(p, "hello world");
    printf("    3: malloc: %s\n", p);
    free(p);
  }

  // 4: see if calloc works
  {
    int *arr = calloc(5, sizeof *arr);
    assert(arr != NULL);
    for (int i = 0; i < 5; i++) {
      assert(arr[i] == 0);
      arr[i] = i + 123;
    }
    printf("    4: values after calloc and +123: ");
    for (int i = 0; i < 5; i++)
      printf("%d ", arr[i]);
    printf("\n");
    free(arr);
  }

  // 5: realloc grows but doesn't lose previous data
  {
    int *arr = calloc(5, sizeof *arr);
    assert(arr != NULL);
    for (int i = 0; i < 5; i++)
      arr[i] = i + 123;

    int *bigger = realloc(arr, 10 * sizeof(int));
    assert(bigger != NULL);
    for (int i = 0; i < 5; i++)
      assert(bigger[i] == i + 123);
    for (int i = 5; i < 10; i++)
      bigger[i] = i + 30;

    printf("    5: realloc grows but what about the data: ");
    for (int i = 0; i < 10; i++)
      printf("%d ", bigger[i]);
    printf("\n");

    free(bigger);
  }

  // 6) smaller with realloc but same ptr
  {
    char *m = malloc(16);
    assert(m);
    strcpy(m, "shrink test");

    char *n = realloc(m, 8);
    assert(n == m);
    assert(strncmp(n, "shrink t", 8) == 0);
    printf("    6: realloc smaller OK\n");

    free(n);
  }

  // 7: two new block and their values
  {
    char *p1 = malloc(32);
    char *p2 = malloc(32);
    assert(p1 && p2);

    memset(p1, 'A', 32);
    memset(p2, 'B', 32);

    for (int i = 0; i < 32; i++) {
      assert(p1[i] == 'A');
      assert(p2[i] == 'B');
    }
    printf("    7: two new blocks and their values OK\n");

    free(p1);
    free(p2);
  }

  // 8: does malloc(0) and free(NULL) work as specified?
  {
    assert(malloc(0) == NULL);
    free(NULL);
    printf("    8: malloc(0) free(NULL) OK\n");
  }

  // 9: allocate a larger block
  {
    size_t large_n = 1000;
    char *big = malloc(large_n);
    assert(big);

    big[0] = 'A';
    big[large_n / 2] = 'B';
    big[large_n - 1] = 'C';

    assert(big[0] == 'A' && big[large_n / 2] == 'B' && big[large_n - 1] == 'C');

    printf("    9: large malloc OK\n");
    free(big);
  }

  printf("All tests passed!\n");
  return 0;
}
