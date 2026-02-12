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
    
    // physical properties
    double radius_equatorial;

    struct {
        double semi_major_axis; // standard Keplerian term
        double period_orbital; // year duration
        double mean_anomaly_epoch; // starting position in orbit
        double eccentricity;
    } orbit;

    // orbital elements
    double inclination; // angle between orbit and reference plane
    double long_asc_node; // longitude of the ascending node
    
    // rotational properties
    double period_rotation; // time per rotation
    double obliquity; // axial tilt

    // dynamic state (runtime)
    struct {
        vec3_t position;
        float anomaly; // position along orbit
        float rotation; // rotation angle
    } state;

    struct planet *parent;
    object_t *object;
} planet_t;

vec3_t r_physics_orbit_to_local(planet_t *planet, vec3_t position);

void r_physics_planet_state_update(planet_t *planet, double time);

struct tm r_physics_clock_to_tm(double time);

#endif // __R_PHYSICS_H__