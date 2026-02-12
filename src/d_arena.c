#include "d_arena.h"

// MEMORY

void d_arena_init(arena_t *arena, void *head, size_t capacity) {
    arena -> head = (uint8_t*) head;
    arena -> size = 0;
    arena -> capacity = capacity;
}

void *d_arena_alloc(arena_t *arena, size_t capacity) {
    size_t aligment = 8;
    size_t padding = (aligment - (arena -> size % aligment)) % aligment;

    QSSERT(arena -> size + capacity <= arena -> capacity, "Game out of memory!\n");

    arena -> size += padding;

    void *head = arena -> head + arena -> size;
    arena -> size += capacity;

    return head;
}

void d_arena_reset(arena_t *arena) {
    arena -> size = 0;
}

void d_arena_stats(arena_t *arena) {
    printf("GAME MEMORY: Used %.4f MB / %.2f MB (%.1f%%)\n", ((float) (arena -> size) / (1024.0f * 1024.0f)), 
        ((float) (arena -> capacity) / (1024.0f * 1024.0f)), (double) arena -> size / (double) arena -> capacity * 100.0);
}