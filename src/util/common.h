#ifndef COMMON_H
#define COMMON_H

#include "object/mesh.h"

#define ASSERT(_e, ...) if (!(_e)) {fprintf(stderr, __VA_ARGS__); exit(1);}

// typedef struct {
//     vec3 pos, normals, uvs;
// } shape_t;

#endif