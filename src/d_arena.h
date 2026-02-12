#ifndef __D_ARENA_H__
#define __D_ARENA_H__

#include <stdint.h>

#include "d_util.h"

// MEMORY

typedef struct arena {
    uint8_t *head;
    size_t size, capacity;
} arena_t;

void d_arena_init(arena_t *arena, void *head, size_t capacity);

void *d_arena_alloc(arena_t *arena, size_t capacity);

void d_arena_reset(arena_t *arena);

void d_arena_stats(arena_t *arena);

#endif // !__D_ARENA_H__