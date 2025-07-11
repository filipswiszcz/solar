#ifndef MESH_H
#define MESH_H

#include <stdint.h>

#include "util/math/quat.h"
#include "util/dyn_array.h"

typedef struct material material_t;

struct material {
    char *name;
    float shininess;
    vec3 ambient, diffuse, specular, emissibity;
    float density, transparency;
    int illumination;
};

typedef struct mesh mesh_t;

struct mesh {
    uint16_t id;
    //farray_t vertices;
    //iarray_t indices;
    material_t material;
};

typedef struct transformation transformation_t;

struct transformation {
    uint16_t id;
    vec3 scale;
    vec3 rot_origin;
    quat rot;
    vec3 translation;
};

typedef struct instance instance_t;

struct instance {
    mesh_t *mesh;
    transformation_t transf;
};

// struct instance {
//     uint16_t mesh_id;
//     uint16_t transform_id;
// };

#endif /* MESH_H */
