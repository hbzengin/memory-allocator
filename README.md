# Memory Allocator

This is a implementation of a simple memory allocator in C, with custom versions of `malloc`, `free`, `calloc`, and `realloc`. It's a simple example of how dynamic memory management works, using the `sbrk` system call and a linked list to keep track of the already-allocated blocks.

### Things To Know

- `malloc`, `free`, `calloc`, and `realloc` are defined in `allocator.h` and implemented in `allocator.c`
- Implementation uses headers to block tracking. So, `sbrk()` calls account for the extra header size.
- After memory locations are `free()`d by the application, they are used by the allocator on a first-match basis
- `main.c` has tests and usage
- I have some `TODO`s for myself at the top of the `allocator.c` file

### Usage

To build and run:

```bash
gcc -o main main.c allocator.c
./main
```

### Inspiration & Acknowledgment

I was inspired to attempt this myself after reading [Dan Luu’s blog post on malloc()](https://danluu.com/malloc-tutorial/). This is a close version based on the ideas and explanation in that article. I am working on expanding this version by completing the `TODO` items.

