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

// typedef struct Planet {
//     const char* name;

//     double orbitRadius;     // km
//     double orbitPeriod;     // seconds
//     double orbitPhase;      // radians

//     double rotationPeriod;  // seconds
//     double axialTilt;       // radians

//     struct Planet* parent;  // NULL for Sun
// } Planet;

// typedef struct {
//     double position[3];     // local position (relative to parent)
//     double orbitAngle;      // radians
//     double spinAngle;       // radians
// } PlanetState;

// void physics_compute_planet_state(
//     const Planet* planet,
//     double simTime,
//     PlanetState* outState
// );

    // context.sun.name = "Sun";
    // context.sun.orbitRadius = 0.0;
    // context.sun.orbitPeriod = 1.0;
    // context.sun.orbitPhase = 0.0;
    // context.sun.rotationPeriod = 25.38 * 86400.0; // seconds
    // context.sun.axialTilt = 0.0;
    // context.sun.parent = NULL;

    // context.earth.name = "Earth";
    // context.earth.orbitRadius = 149597870.7;       // km
    // context.earth.orbitPeriod = 365.256 * 86400.0; // seconds
    // context.earth.orbitPhase = 0.0;
    // context.earth.rotationPeriod = 86400.0;       // seconds
    // context.earth.axialTilt = 23.44 * (R_PI / 180.0);
    // context.earth.parent = &context.sun;

#endif // __R_PHYSICS_H__