#ifndef __R_PHYSICS_H__
#define __R_PHYSICS_H__

#define _POSIX_C_SOURCE 200112L
#include <time.h>

#include "r_renderer.h"

#define R_PHYSICS_ORBIT_SCALE (1.0 / 1e7)
#define R_PHYSICS_PLANET_SCALE (1.0 / 1e5)

#define R_PHYSICS_MINUTE_SECONDS 60.0f
#define R_PHYSICS_HOUR_SECONDS 3600.0f
#define R_PHYSICS_DAY_SECONDS 86400.0f
#define R_PHYSICS_MONTH_SECONDS (30.44f * 86400.0f)
#define R_PHYSICS_YEAR_SECONDS (365.2425f * 86400.0f)

// PLANET

typedef struct planet {
    const char *name;
    vec3_t color;
    
    double radius;

    struct {
        double radius, period, phase;
    } orbit;

    double inclination;
    double node;
    
    double spin;
    double tilt; // radians

    struct {
        vec3_t position;
        float orbit_angle, spin_angle;
    } state;

    struct planet *parent;
    object_t *object;
} planet_t;

vec3_t r_physics_orbit_to_local(planet_t *planet, vec3_t position);

void r_physics_planet_state_update(planet_t *planet, double time);

struct tm r_physics_clock_to_tm(double time);

#endif // __R_PHYSICS_H__