#ifndef DYN_ARRAY_H
#define DYN_ARRAY_H

#include <stdlib.h>

#include "util/common.h"

typedef struct hash_entry hash_entry_t;

struct hash_entry {
    char *key;
    // shape_t value;
    hash_entry_t *next;
};

typedef struct hash_table hash_table_t;

struct hash_table {
    hash_entry_t **entries;
    size_t k;
    size_t capacity;
};

// void hash_table_add(hash_table_t *tab, char *key, shape_t val);

typedef struct farray farray_t;
typedef struct iarray iarray_t;
typedef struct carray carray_t;

struct farray {
    float *values;
    size_t k;
    size_t capacity;
};

struct iarray {
    int *values;
    size_t k;
    size_t capacity;
};

struct carray {
    char **values;
    size_t k;
    size_t capacity;
};

void farray_add(farray_t *arr, float val);

void iarray_add(iarray_t *arr, int val);

// void carray_add(carray_t *arr, char *val);

#endif /* DYN_ARRAY_H */